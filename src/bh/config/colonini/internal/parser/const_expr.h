/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2022  SlashDiablo Team
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

#ifndef BH_CONFIG_COLONINI_INTERNAL_PARSER_CONST_EXPR_H_
#define BH_CONFIG_COLONINI_INTERNAL_PARSER_CONST_EXPR_H_

#include <stddef.h>

#include "bh/config/colonini/internal/lexer/lexer_string.h"
#include "bh/config/colonini/internal/parser/const_expr_type.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct ConstExpr {
  enum ConstExprType type;
  char* expr;
  size_t length;

  const struct LexerString* begin_src;
  const struct LexerString* end_src;
};

/**
 * Initializes a ConstExpr using the strings in the range
 * [begin_str, end_str).
 */
struct ConstExpr* ConstExpr_Init(
    struct ConstExpr* expr,
    const struct LexerString* begin_src,
    const struct LexerString* end_src);

/**
 * Deinitializes a ConstExpr, freeing up resources that were allocated.
 */
void ConstExpr_Deinit(struct ConstExpr* expr);

/**
 * Returns the lexicographical difference between two ConstExpr
 * expressions as strings.
 */
int ConstExpr_CompareExprAsString(
    const struct ConstExpr* left, const struct ConstExpr* right);

/**
 * Returns whether the expressions of two ConstExpr are equivalents.
 */
int ConstExpr_Equal(
    const struct ConstExpr* left, const struct ConstExpr* right);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_PARSER_CONST_EXPR_H_ */
