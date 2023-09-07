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

#include "bh/config/colonini/type/entry.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "bh/config/colonini/type/value.h"

/**
 * External
 */

struct Colonini_Entry* Colonini_Entry_InitDefault(
    struct Colonini_Entry* entry,
    const char* key,
    size_t key_length) {
  struct Colonini_String* key_init_result;

  key_init_result = Colonini_String_Init(&entry->key, key, key_length);
  if (key_init_result == NULL) {
    goto error;
  }

  entry->previous = NULL;
  entry->next = NULL;

  entry->value.type = Colonini_ValueType_kUnspecified;

  return entry;

error:
  return NULL;
}

void Colonini_Entry_Deinit(struct Colonini_Entry* entry) {
  Colonini_Value_Deinit(&entry->value);

  if (entry->previous != NULL) {
    entry->previous->next = entry->next;
    entry->previous = NULL;
  }
  if (entry->next != NULL) {
    entry->next->previous = entry->previous;
    entry->next = NULL;
  }

  Colonini_String_Deinit(&entry->key);
}

int Colonini_Entry_CompareKey(
    const struct Colonini_Entry* left, const struct Colonini_Entry* right) {
  int is_left_shorter;
  size_t min_length;

  int compare_result;

  is_left_shorter = (left->key.length < right->key.length);
  min_length = is_left_shorter ? left->key.length : right->key.length;

  compare_result = memcmp(left->key.str, right->key.str, min_length);
  if (compare_result != 0) {
    return compare_result;
  }

  return is_left_shorter ? -1 : (left->key.length > right->key.length);
}
