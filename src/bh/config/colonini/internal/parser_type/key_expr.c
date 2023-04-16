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

#include "bh/config/colonini/internal/parser_type/key_expr.h"

#include <stddef.h>

#include "bh/config/colonini/internal/parser_type/const_expr.h"
#include "bh/config/colonini/internal/parser_type/subscript.h"

/**
 * External
 */

void KeyExpr_Deinit(struct KeyExpr* expr) {
  while (expr->subscripts_count-- > 0) {
    Subscript_Deinit(&expr->subscripts[expr->subscripts_count]);
  }

  ConstExpr_Deinit(&expr->constexpr);
}
