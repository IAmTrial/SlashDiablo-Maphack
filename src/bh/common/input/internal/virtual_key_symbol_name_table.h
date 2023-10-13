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

#ifndef BH_COMMON_INPUT_INTERNAL_VIRTUAL_KEY_SYMBOL_NAME_TABLE_H_
#define BH_COMMON_INPUT_INTERNAL_VIRTUAL_KEY_SYMBOL_NAME_TABLE_H_

#include <stddef.h>

#include "bh/common/data_struct/red_black_tree.h"
#include "bh/common/input/virtual_key.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct VirtualKeySymbolNameTableEntry {
  const char* symbol_name;
  size_t symbol_name_length;

  const struct Input_VirtualKey* virtual_key;
};

int VirtualKeySymbolNameTableEntry_Compare(
    const struct VirtualKeySymbolNameTableEntry* left,
    const struct VirtualKeySymbolNameTableEntry* right);

struct VirtualKeySymbolNameTable {
  struct VirtualKeySymbolNameTableEntry* entries;
  size_t count;

  struct RedBlackTree tree;
};

struct VirtualKeySymbolNameTable* VirtualKeySymbolNameTable_Init(
    struct VirtualKeySymbolNameTable* table, size_t capacity);

void VirtualKeySymbolNameTable_Deinit(struct VirtualKeySymbolNameTable* table);

const struct Input_VirtualKey* VirtualKeySymbolNameTable_Get(
    struct VirtualKeySymbolNameTable* table,
    const char* symbol_name,
    size_t symbol_name_length);

size_t VirtualKeySymbolNameTable_Insert(
    struct VirtualKeySymbolNameTable* table,
    const struct Input_VirtualKey* virtual_key);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_INPUT_INTERNAL_VIRTUAL_KEY_SYMBOL_NAME_TABLE_H_ */
