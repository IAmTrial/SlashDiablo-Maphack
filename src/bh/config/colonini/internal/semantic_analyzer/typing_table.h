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

#ifndef BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_TYPING_TABLE_H_
#define BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_TYPING_TABLE_H_

#include <stddef.h>

#include "bh/common/data_struct/red_black_tree.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/parser_line.h"
#include "bh/config/colonini/internal/semantic_analyzer/typing.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct TypingTable {
  size_t capacity;
  size_t count;
  struct Typing* typings;
  struct RedBlackTree tree;
};

struct TypingTable* TypingTable_Init(
    struct TypingTable* table, size_t capacity);

void TypingTable_Deinit(struct TypingTable* table);

struct Typing* TypingTable_InsertFromLine(
    struct TypingTable* table, const struct ParserLine* line);

struct Typing* TypingTable_InsertOrResolveFromLine(
    struct TypingTable* table, const struct ParserLine* line);

struct Typing* TypingTable_SearchWithPrimaryKey(
    const struct TypingTable* table, const struct ConstExpr* expr);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_TYPING_TABLE_H_ */
