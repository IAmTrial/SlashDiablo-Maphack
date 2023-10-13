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

/**
 * BH
 * Copyright (C) 2011  McGod
 *
 * All rights reserved.
 */

#include "bh/common/input/internal/virtual_key_symbol_name_table.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "bh/common/data_struct/red_black_tree.h"
#include "bh/common/input/virtual_key.h"

static int VirtualKeySymbolNameTableEntry_CompareAsVoid(
    const struct VirtualKeySymbolNameTableEntry* left,
    const struct VirtualKeySymbolNameTableEntry* right) {
  return VirtualKeySymbolNameTableEntry_Compare(left, right);
}

/**
 * External
 */

int VirtualKeySymbolNameTableEntry_Compare(
    const struct VirtualKeySymbolNameTableEntry* left,
    const struct VirtualKeySymbolNameTableEntry* right) {
  int is_left_shorter;
  size_t shorter_length;
  int compare_result;

  is_left_shorter = left->symbol_name_length < right->symbol_name_length;
  shorter_length =
      is_left_shorter ? left->symbol_name_length : right->symbol_name_length;
  compare_result =
      memcmp(left->symbol_name, right->symbol_name, shorter_length);
  if (compare_result != 0) {
    return compare_result;
  }

  return is_left_shorter
      ? -1
      : left->symbol_name_length > right->symbol_name_length;
}

struct VirtualKeySymbolNameTable* VirtualKeySymbolNameTable_Init(
    struct VirtualKeySymbolNameTable* table, size_t capacity) {
  struct RedBlackTree* tree_init_result;

  table->count = 0;
  table->entries = malloc(capacity * sizeof(table->entries[0]));
  if (table->entries == NULL) {
    goto error;
  }

  tree_init_result = RedBlackTree_Init(&table->tree);
  if (tree_init_result == NULL) {
    goto error_free_entries;
  }

  return table;

error_free_entries:
  free(table->entries);

error:
  return NULL;
}

void VirtualKeySymbolNameTable_Deinit(struct VirtualKeySymbolNameTable* table) {
  RedBlackTree_Deinit(&table->tree);
  free(table->entries);
  table->entries = NULL;
}

const struct Input_VirtualKey* VirtualKeySymbolNameTable_Get(
    struct VirtualKeySymbolNameTable* table,
    const char* symbol_name,
    size_t symbol_name_length) {
  struct VirtualKeySymbolNameTableEntry search_key;
  const struct RedBlackNode* search_result;
  const struct VirtualKeySymbolNameTableEntry* result_entry;

  search_key.symbol_name = symbol_name;
  search_key.symbol_name_length = symbol_name_length;

  search_result =
      RedBlackTree_Find(
          &table->tree,
          &search_key,
          &VirtualKeySymbolNameTableEntry_CompareAsVoid);
  if (search_result == NULL) {
    return NULL;
  }

  result_entry = search_result->ptr;

  return result_entry->virtual_key;
}

size_t VirtualKeySymbolNameTable_Insert(
    struct VirtualKeySymbolNameTable* table,
    const struct Input_VirtualKey* virtual_key) {
  struct VirtualKeySymbolNameTableEntry* current_entry;

  current_entry = &table->entries[table->count];

  current_entry->symbol_name = virtual_key->symbol_name;
  current_entry->symbol_name_length = virtual_key->symbol_name_length;
  current_entry->virtual_key = virtual_key;
  RedBlackTree_Insert(
      &table->tree,
      current_entry,
      &VirtualKeySymbolNameTableEntry_CompareAsVoid);
  ++table->count;

  /* Add the old symbol, if different from the new symbol name. */
  if (virtual_key->symbol_name_length == virtual_key->old_symbol_name_length) {
    int compare_result;

    compare_result =
        memcmp(
            virtual_key->symbol_name,
            virtual_key->old_symbol_name,
            virtual_key->symbol_name_length);
    if (compare_result == 0) {
      return 1;
    }
  }

  current_entry = &table->entries[table->count];

  current_entry->symbol_name = virtual_key->old_symbol_name;
  current_entry->symbol_name_length = virtual_key->old_symbol_name_length;
  current_entry->virtual_key = virtual_key;
  RedBlackTree_Insert(
      &table->tree,
      current_entry,
      &VirtualKeySymbolNameTableEntry_CompareAsVoid);
  ++table->count;

  return 2;
}
