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

#ifndef BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_KEY_TABLE_H_
#define BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_KEY_TABLE_H_

#include <stddef.h>

#include "bh/common/data_struct/red_black_tree.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/key_expr.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct KeyTableEntry;

struct KeyTable {
  struct KeyTableEntry* tail;
  size_t count;
  struct RedBlackTree tree;
};

struct KeyTableEntry {
  const struct ConstExpr* expr;
  struct KeyTable table;

  struct KeyTableEntry* previous;
};

struct KeyTable* KeyTable_Init(struct KeyTable* table);

void KeyTable_Deinit(struct KeyTable* table);

struct KeyTable* KeyTable_FindFromKey(
    const struct KeyTable* table, const struct ConstExpr* key);

struct KeyTable* KeyTable_FindFromKeys(
    const struct KeyTable* table, const struct KeyExpr* keys);

size_t KeyTable_InsertFromKey(
    struct KeyTable* table, const struct ConstExpr* key);

/**
 * Inserts the key and subkeys of a key expression into the key table.
 * Returns the total count of key and subkeys inserted.
 */
size_t KeyTable_InsertFromKeys(
    struct KeyTable* table, const struct KeyExpr* keys);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_KEY_TABLE_H_ */
