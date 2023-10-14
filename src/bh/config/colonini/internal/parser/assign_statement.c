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

#include "bh/config/colonini/internal/parser/assign_statement.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "bh/config/colonini/internal/lexer/lexer_string.h"
#include "bh/config/colonini/internal/parser/key_expr.h"
#include "bh/config/colonini/internal/parser/value_expr.h"

static const char kColon[] = ":";

/**
 * External
 */

struct AssignStatement* AssignStatement_Parse(
    struct AssignStatement* statement,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column) {
  size_t dummy_error_column;
  int peek_result;
  struct KeyExpr* parse_key_result;
  struct ValueExpr* parse_value_result;

  const struct LexerString* first_token;
  struct KeyExpr* key;
  const struct LexerString* key_end_src;
  const struct LexerString* colon_op;

  const struct LexerString* value_begin_src;
  const struct LexerString* value_end_src;
  struct ValueExpr* value_expr;

  if (error_column == NULL) {
    error_column = &dummy_error_column;
  }

  if (begin_src == NULL || end_src == NULL) {
    *error_column = 1;
    return NULL;
  }

  first_token = LexerString_CeilToken(begin_src);
  if (first_token == NULL || first_token >= end_src) {
    *error_column = begin_src->line_index + 1;
    return NULL;
  }

  key = &statement->key_expr;
  parse_key_result = KeyExpr_Parse(key, begin_src, end_src, error_column);
  if (parse_key_result == NULL) {
    return NULL;
  }

  key_end_src =
      (key->subscript_count == 0)
          ? key->primary.end_src
          : key->subscripts[key->subscript_count - 1].end_src;
  if (key_end_src == NULL || key_end_src >= end_src) {
    *error_column = first_token->line_index + 1;
    return NULL;
  }

  colon_op = LexerString_CeilToken(key_end_src);
  if (colon_op == NULL || colon_op >= end_src) {
    return NULL;
  }

  if (colon_op->str_length != 1
      || memcmp(colon_op->str, kColon, sizeof(kColon)) != 0) {
    *error_column = colon_op->line_index + 1;
    return NULL;
  }

  value_begin_src = colon_op->next_token;
  parse_value_result =
      ValueExpr_Parse(
          &statement->value_expr,
          value_begin_src,
          end_src,
          error_column);
  if (parse_value_result == NULL) {
    return NULL;
  }

  return statement;
}

void AssignStatement_Deinit(struct AssignStatement* assign_statement) {
  ValueExpr_Deinit(&assign_statement->value_expr);
  KeyExpr_Deinit(&assign_statement->key_expr);
}
