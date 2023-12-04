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
#include "bh/config/colonini/type/entry.h"
#include "bh/config/colonini/type/value.h"

static int Colonini_Entry_CompareKeyAsVoid(
    const void* left, const void* right) {
  return Colonini_Entry_CompareKey(left, right);
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
  struct Colonini_Value* map_value;
  struct Colonini_Value* value_init_result;

  map_value = Colonini_Map_PutEmpty(map, key, key_length);
  if (map_value == NULL) {
    goto error;
  }

  value_init_result = Colonini_Value_InitAsBoolean(map_value, value);
  if (value_init_result == NULL) {
    goto error;
  }

  return &map_value->variant.as_data.variant.as_boolean;

error:
  return NULL;
}

struct Colonini_Value* Colonini_Map_PutEmpty(
    struct Colonini_Map* map, const char* key, size_t key_length) {
  struct Colonini_Entry* entry_init_result;

  int contained_entry;
  struct Colonini_Entry* entry;
  int entry_inserted;

  /* Reuse the old entry, if possible. */
  entry = Colonini_Map_Find(map, key, key_length);
  contained_entry = (entry != NULL);
  if (contained_entry) {
    struct Colonini_Value* value_init_result;

    Colonini_Value_Deinit(&entry->value);
    value_init_result = Colonini_Value_InitAsEmpty(&entry->value);
    if (value_init_result == NULL) {
      goto error_free_entry;
    }
    return &entry->value;
  }

  entry = malloc(sizeof(*entry));
  if (entry == NULL) {
    goto error;
  }

  entry_init_result = Colonini_Entry_InitEmpty(entry, key, key_length);
  if (entry_init_result == NULL) {
    goto error_free_entry;
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

  entry_inserted =
      RedBlackTree_Insert(
          &map->tree, entry, &Colonini_Entry_CompareKeyAsVoid);
  if (!entry_inserted) {
    goto error_free_entry;
  }
  ++map->count;

  return &entry->value;

error_free_entry:
  if (contained_entry) {
    RedBlackTree_Remove(&map->tree, entry, &Colonini_Entry_CompareKeyAsVoid);
  }
  free(entry);

error:
  return NULL;
}

unsigned int* Colonini_Map_PutInteger(
    struct Colonini_Map* map,
    const char* key,
    size_t key_length,
    unsigned int value) {
  struct Colonini_Value* map_value;
  struct Colonini_Value* value_init_result;

  map_value = Colonini_Map_PutEmpty(map, key, key_length);
  if (map_value == NULL) {
    goto error;
  }

  value_init_result = Colonini_Value_InitAsInteger(map_value, value);
  if (value_init_result == NULL) {
    goto error;
  }

  return &map_value->variant.as_data.variant.as_integer;

error:
  return NULL;
}

struct Colonini_Map* Colonini_Map_PutMap(
    struct Colonini_Map* map, const char* key, size_t key_length) {
  struct Colonini_Value* map_value;
  struct Colonini_Value* value_init_result;

  map_value = Colonini_Map_PutEmpty(map, key, key_length);
  if (map_value == NULL) {
    goto error;
  }

  value_init_result = Colonini_Value_InitAsMap(map_value);
  if (value_init_result == NULL) {
    goto error;
  }

  return &map_value->variant.as_map;

error:
  return NULL;
}

struct Colonini_String* Colonini_Map_PutString(
    struct Colonini_Map* map,
    const char* key,
    size_t key_length,
    const char* str,
    size_t str_length) {
  struct Colonini_Value* map_value;
  struct Colonini_Value* value_init_result;

  map_value = Colonini_Map_PutEmpty(map, key, key_length);
  if (map_value == NULL) {
    goto error;
  }

  value_init_result = Colonini_Value_InitAsString(map_value, str, str_length);
  if (value_init_result == NULL) {
    goto error;
  }

  return &map_value->variant.as_data.variant.as_string;

error:
  return NULL;
}

struct Colonini_Toggle* Colonini_Map_PutToggle(
    struct Colonini_Map* map,
    const char* key,
    size_t key_length,
    unsigned char enabled,
    BYTE key_code) {
  struct Colonini_Value* map_value;
  struct Colonini_Value* value_init_result;

  map_value = Colonini_Map_PutEmpty(map, key, key_length);
  if (map_value == NULL) {
    goto error;
  }

  value_init_result = Colonini_Value_InitAsToggle(map_value, enabled, key_code);
  if (value_init_result == NULL) {
    goto error;
  }

  return &map_value->variant.as_data.variant.as_toggle;

error:
  return NULL;
}

int Colonini_Map_Remove(
    struct Colonini_Map* map, const char* key, size_t key_length) {
  struct Colonini_Entry search_key;
  struct Colonini_Entry* entry;

  entry = Colonini_Map_Find(map, key, key_length);
  if (entry == NULL) {
    return 0;
  }

  --map->count;

  search_key.key.str = (char*)key;
  search_key.key.length = key_length;
  RedBlackTree_Remove(
      &map->tree, &search_key, &Colonini_Entry_CompareKeyAsVoid);

  if (entry == map->head_entry) {
    map->head_entry = entry->next;
  }
  if (entry == map->tail_entry) {
    map->tail_entry = entry->previous;
  }

  Colonini_Entry_Deinit(entry);

  return 1;
}
