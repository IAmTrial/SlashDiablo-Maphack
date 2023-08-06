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

#ifndef BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_TYPING_H_
#define BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_TYPING_H_

#include <stddef.h>

#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/const_expr_type.h"
#include "bh/config/colonini/internal/parser/parser_line.h"
#include "bh/config/colonini/internal/parser/value_expr_type.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct Typing {
  const struct ConstExpr* key_name;

  /** Types for subkeys. Primary key is always string. */
  enum ConstExprType* subkey_types;
  size_t subkey_type_count;

  enum ValueExprType value_type;
  enum ConstExprType value_as_constexpr_type;
};

struct Typing* Typing_Init(
    struct Typing* typing, const struct ParserLine* line);

void Typing_Deinit(struct Typing* typing);

int Typing_CompareKeyName(
    const struct Typing* left, const struct Typing* right);

int Typing_Equal(const struct Typing* left, const struct Typing* right);

/**
 * Resolves the differences between two Typing, converting to
 * string type as needed. Returns a non-zero value on success, or zero
 * on failure.
 */
int Typing_ResolveDiff(struct Typing* left, struct Typing* right);

/**
 * Determines the type differences between a Typing and a ParserLine,
 * changing the typing to string type as needed. Returns a non-zero
 * value on success, or zero on failure.
 */
int Typing_ResolveLineDiff(
    struct Typing* typing, const struct ParserLine* line);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_TYPING_H_ */
