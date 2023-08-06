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

#ifndef BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_VARIABLE_H_
#define BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_VARIABLE_H_

#include <stddef.h>

#include "bh/config/colonini/internal/parser/key_expr.h"
#include "bh/config/colonini/internal/parser/parser_line.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct Variable {
  size_t line_number;
  const struct KeyExpr* key_expr;
};

struct Variable* Variable_Init(
    struct Variable* variable, const struct ParserLine* line);

void Variable_Deinit(struct Variable* variable);

int Variable_CompareKeysAsStrings(
    const struct Variable* left, const struct Variable* right);

/**
 * Returns whether the key and subkeys of two Variable are equivalents.
 */
int Variable_EqualKeys(
    const struct Variable* left, const struct Variable* right);

/**
 * Returns whether the primary key name of two Variable are
 * equivalents.
 */
int Variable_EqualName(
    const struct Variable* left, const struct Variable* right);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_VARIABLE_H_ */
