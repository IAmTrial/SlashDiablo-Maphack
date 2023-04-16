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

#ifndef BH_CONFIG_COLONINI_INTERNAL_PARSER_TYPE_ASSIGN_STATEMENT_H_
#define BH_CONFIG_COLONINI_INTERNAL_PARSER_TYPE_ASSIGN_STATEMENT_H_

#include "bh/config/colonini/internal/parser_type/key_expr.h"
#include "bh/config/colonini/internal/parser_type/value_expr.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct AssignStatement {
  struct KeyExpr key_expr;
  struct ValueExpr value_expr;
};

/**
 * Deinitializes a AssignStatement, freeing up resources that were
 * allocated.
 */
void AssignStatement_Deinit(struct AssignStatement* assign_statement);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_PARSER_TYPE_ASSIGN_STATEMENT_H_ */
