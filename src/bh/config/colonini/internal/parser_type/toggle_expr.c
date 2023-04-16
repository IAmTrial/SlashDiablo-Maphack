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

#include "bh/config/colonini/internal/parser_type/toggle_expr.h"

#include <stddef.h>
#include <string.h>

#include "bh/config/colonini/internal/lexer.h"
#include "bh/config/colonini/internal/parser_type/const_expr.h"

static const char kComma[] = ",";

/**
 * External
 */

int ToggleExpr_IsValid(
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column) {
  const struct LexerString* enabled_begin_src;
  enum ConstExprType enabled_type;
  const struct LexerString* comma_op;
  const struct LexerString* input_begin_src;
  enum ConstExprType input_type;

  /* Validate the enabled expression. */
  enabled_begin_src = begin_src;
  if (enabled_begin_src == NULL || enabled_begin_src == end_src) {
    *error_column = 0;
    return 0;
  }

  enabled_type =
      ConstExprType_MatchString(
          enabled_begin_src->str, enabled_begin_src->str_length);
  if (enabled_type != ConstExprType_kBoolean) {
    *error_column = enabled_begin_src->line_index;
    return 0;
  }

  /* Validate the , operator. */
  comma_op = enabled_begin_src->next_token;
  if (comma_op == NULL || comma_op == end_src) {
    *error_column = enabled_begin_src->line_index;
    return 0;
  }

  if (comma_op->str_length != 1
      || memcmp(comma_op->str, kComma, sizeof(kComma)) != 0) {
    *error_column = comma_op->line_index;
    return 0;
  }

  /* Validate the input expression. */
  input_begin_src = comma_op->next_token;
  if (input_begin_src == NULL || input_begin_src == end_src) {
    *error_column = comma_op->line_index;
    return 0;
  }

  if (&input_begin_src[1] != end_src) {
    *error_column = input_begin_src->line_index;
    return 0;
  }

  input_type =
      ConstExprType_MatchString(
          input_begin_src->str, input_begin_src->str_length);
  if (input_type != ConstExprType_kString) {
    *error_column = enabled_begin_src->line_index;
    return 0;
  }

  return 1;
}

struct ToggleExpr* ToggleExpr_Parse(
    struct ToggleExpr* expr,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column) {
  struct ConstExpr* enabled_init_result;
  struct ConstExpr* input_init_result;

  const struct LexerString* enabled_begin_src;
  const struct LexerString* comma_op;
  const struct LexerString* input_begin_src;

  enabled_begin_src = begin_src;
  comma_op = enabled_begin_src->next_token;
  input_begin_src = comma_op->next_token;

  enabled_init_result =
      ConstExpr_Init(
          &expr->enabled_expr, enabled_begin_src, &enabled_begin_src[1]);
  if (enabled_init_result == NULL) {
    *error_column = 0;
    goto error;
  }

  input_init_result =
      ConstExpr_Init(
          &expr->input_expr, input_begin_src, &input_begin_src[1]);
  if (input_init_result == NULL) {
    *error_column = 0;
    goto error;
  }

  return expr;

error:
  return NULL;
}

void ToggleExpr_Deinit(struct ToggleExpr* expr) {
  ConstExpr_Deinit(&expr->input_expr);
  ConstExpr_Deinit(&expr->enabled_expr);
}
