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

#ifndef BH_CONFIG_COLONINI_INTERNAL_PARSER_KEY_EXPR_H_
#define BH_CONFIG_COLONINI_INTERNAL_PARSER_KEY_EXPR_H_

#include <stddef.h>

#include "bh/config/colonini/internal/lexer/lexer_string.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/subscript.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct KeyExpr {
  struct ConstExpr primary;
  struct Subscript* subscripts;
  size_t subscript_count;
};

/**
 * Starting from begin_src, parses a KeyExpr constituted of tokens and
 * whitespace no further than end_src. Returns a non-NULL on success,
 * or else returns NULL and sets error column.
 */
struct KeyExpr* KeyExpr_Parse(
    struct KeyExpr* expr,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column);

/**
 * Deinitializes a KeyExpr, freeing up resources that were allocated.
 */
void KeyExpr_Deinit(struct KeyExpr* expr);

int KeyExpr_CompareKeysAsStrings(
    const struct KeyExpr* left, const struct KeyExpr* right);

/**
 * Returns whether the keys and subkeys of two KeyExpr are
 * equivalents.
 */
int KeyExpr_Equal(
    const struct KeyExpr* left, const struct KeyExpr* right);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_PARSER_KEY_EXPR_H_ */
