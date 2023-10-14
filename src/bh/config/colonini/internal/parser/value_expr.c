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

#include "bh/config/colonini/internal/parser/value_expr.h"

#include <assert.h>

#include "bh/config/colonini/internal/lexer/lexer_string.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/toggle_expr.h"
#include "bh/config/colonini/internal/parser/value_expr_type.h"

/**
 * External
 */

struct ValueExpr* ValueExpr_Parse(
    struct ValueExpr* expr,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column) {
  size_t dummy_error_column;
  struct ConstExpr* init_result;

  if (error_column == NULL) {
    error_column = &dummy_error_column;
  }

  if (end_src == NULL) {
    *error_column = 0;
    return NULL;
  }

  /* If there is no value, assume it is Empty type. */
  if (begin_src == NULL) {
    expr->type = ValueExprType_kEmpty;
    return expr;
  }

  /* Attempt to parse as ToggleExpr. */
  if (ToggleExpr_IsValid(begin_src, end_src, error_column)) {
    struct ToggleExpr* init_result;

    expr->type = ValueExprType_kToggle;
    init_result =
        ToggleExpr_Parse(
            &expr->variant.as_toggleexpr, begin_src, end_src, error_column);
    if (init_result == NULL) {
      return NULL;
    }

    return expr;
  }

  /* Parse as ConstExpr. */
  expr->type = ValueExprType_kConst;
  init_result =
      ConstExpr_Init(&expr->variant.as_constexpr, begin_src, end_src);
  if (init_result == NULL) {
    *error_column = 0;
    goto error;
  }

  return expr;

error:
  return NULL;
}

void ValueExpr_Deinit(struct ValueExpr* expr) {
  switch (expr->type) {
    case ValueExprType_kEmpty: {
      break;
    }

    case ValueExprType_kConst: {
      ConstExpr_Deinit(&expr->variant.as_constexpr);
      break;
    }

    case ValueExprType_kToggle: {
      ToggleExpr_Deinit(&expr->variant.as_toggleexpr);
      break;
    }

    default: {
      assert(0 && "This should never happen.");
      return;
    }
  }
  expr->type = ValueExprType_kUnspecified;
}
