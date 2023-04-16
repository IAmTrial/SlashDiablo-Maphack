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

#include "bh/config/colonini/internal/parser_type/assign_statement.h"

#include "bh/config/colonini/internal/parser_type/key_expr.h"
#include "bh/config/colonini/internal/parser_type/value_expr.h"

/**
 * External
 */

void AssignStatement_Deinit(struct AssignStatement* assign_statement) {
  ValueExpr_Deinit(&assign_statement->value_expr);
  KeyExpr_Deinit(&assign_statement->key_expr);
}
