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

#include "bh/config/colonini/internal/parser/const_expr.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "bh/config/colonini/internal/lexer/lexer_string.h"
#include "bh/config/colonini/internal/parser/const_expr_type.h"

/**
 * External
 */

struct ConstExpr* ConstExpr_Init(
    struct ConstExpr* expr,
    const struct LexerString* begin_src,
    const struct LexerString* end_src) {
  const struct LexerString* current_src;
  size_t total_length;

  expr->begin_src = begin_src;
  expr->end_src = end_src;

  /* Determine the size needed to store the full string. */
  total_length = 0;
  for (current_src = begin_src; current_src != end_src; ++current_src) {
    total_length += current_src->str_length;
  }

  /* Allocate space for the expression. */
  expr->expr = malloc((total_length + 1) * sizeof(expr->expr[0]));
  if (expr->expr == NULL) {
    goto error;
  }

  /* Copy the strings into one expression. */
  expr->length = 0;
  for (current_src = begin_src; current_src != end_src; ++current_src) {
    memcpy(
        &expr->expr[expr->length], current_src->str, current_src->str_length);
    expr->length += current_src->str_length;
  }
  expr->expr[expr->length] = '\0';

  expr->type = ConstExprType_MatchString(expr->expr, expr->length);

  return expr;

error:
  return NULL;
}

void ConstExpr_Deinit(struct ConstExpr* expr) {
  free(expr->expr);
  expr->expr = NULL;
  expr->length = 0;
  expr->end_src = NULL;
  expr->begin_src = NULL;
  expr->type = ConstExprType_kUnspecified;
}

int ConstExpr_CompareExprAsString(
    const struct ConstExpr* left, const struct ConstExpr* right) {
  int is_left_shorter;
  size_t max_length;
  int cmp_result;

  is_left_shorter = (left->length < right->length);
  max_length = is_left_shorter ? left->length : right->length;
  cmp_result = memcmp(left->expr, right->expr, max_length);
  if (cmp_result != 0) {
    return cmp_result;
  }

  return is_left_shorter ? -1 : left->length - right->length;
}

int ConstExpr_Equal(
    const struct ConstExpr* left, const struct ConstExpr* right) {
  if (left->type != right->type) {
    return 0;
  }

  if (left->length != right->length) {
    return 0;
  }

  return (memcmp(left->expr, right->expr, left->length) == 0);
}
