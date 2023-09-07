/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2023  SlashDiablo Team
 *
 * This file is part of SlashDiablo Maphack.
 *
 * SlashDiablo Maphack is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "bh/config/colonini/type/map.h"

#include <stddef.h>
#include <stdlib.h>

#include "bh/common/data_struct/red_black_tree.h"
#include "bh/config/colonini/internal/parser.h"
#include "bh/config/colonini/type/entry.h"
#include "bh/config/colonini/type/value_type.h"

static int Colonini_Entry_CompareKeyAsVoid(
    const void* left, const void* right) {
  return Colonini_Entry_CompareKey(left, right);
}

static struct Colonini_Entry* Colonini_Map_PutEmpty(
    struct Colonini_Map* map,
    const char* key,
    size_t key_length) {
  struct Colonini_Entry* entry_init_result;

  int contained_entry;
  struct Colonini_Entry* entry;

  /* Reuse the old entry, if possible. */
  entry = Colonini_Map_Find(map, key, key_length);
  contained_entry = (entry != NULL);
  if (contained_entry) {
    Colonini_Value_Deinit(&entry->value);
  } else {
    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
      goto error;
    }

    entry_init_result = Colonini_Entry_InitDefault(entry, key, key_length);
    if (entry_init_result == NULL) {
      goto error_free_entry;
    }
  }

  /* Initialize an entry's key and previous/next pointers. */
  if (map->head_entry == NULL) {
    map->head_entry = entry;
  } else {
    map->tail_entry->next = entry;
  }
  entry->previous = map->tail_entry;
  entry->next = NULL;
  map->tail_entry = entry;

  if (!contained_entry) {
    int entry_inserted;

    entry_inserted =
        RedBlackTree_Insert(
            &map->tree, entry, &Colonini_Entry_CompareKeyAsVoid);
    if (!entry_inserted) {
      goto error_free_entry;
    }
  }

  return entry;

error_free_entry:
  if (contained_entry) {
    RedBlackTree_Remove(&map->tree, entry, &Colonini_Entry_CompareKeyAsVoid);
  }
  free(entry);

error:
  return NULL;
}

/**
 * External
 */

struct Colonini_Map* Colonini_Map_Init(struct Colonini_Map* map) {
  struct RedBlackTree* tree_init_result;

  map->count = 0;
  map->head_entry = NULL;
  map->tail_entry = NULL;

  tree_init_result = RedBlackTree_Init(&map->tree);
  if (tree_init_result == NULL) {
    goto error;
  }

  return map;

error:
  return NULL;
}

void Colonini_Map_Deinit(struct Colonini_Map* map) {
  RedBlackTree_Deinit(&map->tree);

  while (map->tail_entry != NULL) {
    struct Colonini_Entry* current;

    --map->count;
    current = map->tail_entry;
    map->tail_entry = map->tail_entry->previous;
    Colonini_Entry_Deinit(current);
  }
  map->head_entry = NULL;
}

struct Colonini_Entry* Colonini_Map_Find(
    const struct Colonini_Map* map, const char* key, size_t key_length) {
  struct Colonini_Entry find_key;
  struct RedBlackNode* find_result;

  find_key.key.str = (char*)key;
  find_key.key.length = key_length;

  find_result =
      RedBlackTree_Find(
          &map->tree, &find_key, &Colonini_Entry_CompareKeyAsVoid);
  if (find_result == NULL) {
    return NULL;
  }

  return find_result->ptr;
}

struct Colonini_Value* Colonini_Map_Get(
    const struct Colonini_Map* map, const char* key, size_t key_length) {
  struct Colonini_Entry* find_result;

  find_result = Colonini_Map_Find(map, key, key_length);
  if (find_result == NULL) {
    return NULL;
  }

  return &find_result->value;
}

unsigned char* Colonini_Map_PutBoolean(
    struct Colonini_Map* map,
    const char* key,
    size_t key_length,
    unsigned char value) {
  struct Colonini_Entry* entry;
  struct Colonini_Value* entry_value;

  entry = Colonini_Map_PutEmpty(map, key, key_length);
  if (entry == NULL) {
    goto error;
  }

  entry_value = Colonini_Value_InitAsBoolean(&entry->value, value);
  if (entry_value == NULL) {
    goto error;
  }

  return &entry_value->variant.as_data.variant.as_boolean;

error:
  return NULL;
}

unsigned int* Colonini_Map_PutInteger(
    struct Colonini_Map* map,
    const char* key,
    size_t key_length,
    unsigned int value) {
  struct Colonini_Entry* entry;
  struct Colonini_Value* entry_value;

  entry = Colonini_Map_PutEmpty(map, key, key_length);
  if (entry == NULL) {
    goto error;
  }

  entry_value = Colonini_Value_InitAsInteger(&entry->value, value);
  if (entry_value == NULL) {
    goto error;
  }

  return &entry_value->variant.as_data.variant.as_integer;

error:
  return NULL;
}

struct Colonini_String* Colonini_Map_PutString(
    struct Colonini_Map* map,
    const char* key,
    size_t key_length,
    const char* str,
    size_t str_length) {
  struct Colonini_Entry* entry;
  struct Colonini_Value* entry_value;

  entry = Colonini_Map_PutEmpty(map, key, key_length);
  if (entry == NULL) {
    goto error;
  }

  entry_value = Colonini_Value_InitAsString(&entry->value, str, str_length);
  if (entry_value == NULL) {
    goto error;
  }

  return &entry_value->variant.as_data.variant.as_string;

error:
  return NULL;
}
