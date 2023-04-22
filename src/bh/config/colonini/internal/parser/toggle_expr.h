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

#ifndef BH_CONFIG_COLONINI_INTERNAL_PARSER_TOGGLE_EXPR_H_
#define BH_CONFIG_COLONINI_INTERNAL_PARSER_TOGGLE_EXPR_H_

#include "bh/config/colonini/internal/lexer/lexer_string.h"
#include "bh/config/colonini/internal/parser/const_expr.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct ToggleExpr {
  struct ConstExpr enabled_expr;
  struct ConstExpr input_expr;
};

/**
 * Checks whether the sequence of LexerString in the range
 * [begin_src, end_src) make up a valid ToggleExpr. Returns a non-zero
 * value if valid, or else returns zero and sets error column.
 */
int ToggleExpr_IsValid(
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column);

/**
 * Parses a ToggleExpr in the range [begin_src, end_src). Returns NULL
 * and sets error column on failure.
 *
 * Does not perform input validation. Call IsValid prior to calling
 * Parse.
 */
struct ToggleExpr* ToggleExpr_Parse(
    struct ToggleExpr* expr,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column);

/**
 * Deinitializes a ToggleExpr, freeing up resources that were allocated.
 */
void ToggleExpr_Deinit(struct ToggleExpr* expr);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_PARSER_TOGGLE_EXPR_H_ */