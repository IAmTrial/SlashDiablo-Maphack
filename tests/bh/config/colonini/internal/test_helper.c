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

#include "bh/config/colonini/internal/test_helper.h"

#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include "bh/config/colonini/internal/lexer.h"
#include "bh/config/colonini/internal/lexer/lexer_string.h"
#include "bh/config/colonini/internal/parser.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/const_expr_type.h"
#include "bh/config/colonini/internal/parser/key_expr.h"
#include "bh/config/colonini/internal/parser/subscript.h"
#include "bh/config/colonini/internal/parser/value_expr.h"
#include "bh/config/colonini/internal/parser/value_expr_type.h"

void LexerLineSetUp(
    struct LexerLine* line, struct LexerString* strs, size_t count, ...) {
  va_list args;

  va_start(args, count);
  LexerLineVSetUp(line, strs, count, args);
  va_end(args);
}

void LexerLineVSetUp(
    struct LexerLine* line,
    struct LexerString* strs,
    size_t count,
    va_list args) {
  size_t i;
  size_t total_line_index;

  line->strs = strs;
  line->strs_count = count;

  line->tokens_count = 0;
  total_line_index = 0;
  line->first_token = NULL;
  line->last_token = NULL;
  for (i = 0; i < line->strs_count; ++i) {
    int is_token;

    strs[i].str = va_arg(args, char*);
    strs[i].str_length = strlen(strs[i].str);
    strs[i].line_index = total_line_index;
    strs[i].previous_token = line->last_token;
    strs[i].next_token = NULL;

    is_token = va_arg(args, int);
    if (is_token) {
      size_t ii;

      if (line->first_token == NULL) {
        line->first_token = &strs[i];
      }
      ++line->tokens_count;

      for (ii = (line->last_token == NULL)
              ? 0
              : line->last_token - strs;
          ii < i;
          ++ii) {
        strs[ii].next_token = &strs[i];
      }
      line->last_token = &strs[i];
    }

    total_line_index += strs[i].str_length;
  }
}

void ParserLineBasicSetUp(
    struct ParserLine* line,
    const char* value,
    const char* key,
    struct Subscript* subscripts,
    size_t subscripts_count,
    ...) {
  va_list args;

  va_start(args, subscripts_count);
  ParserLineBasicVSetUp(line, value, key, subscripts, subscripts_count, args);
  va_end(args);
}

void ParserLineBasicVSetUp(
    struct ParserLine* line,
    const char* value,
    const char* key,
    struct Subscript* subscripts,
    size_t subscripts_count,
    va_list args) {
  struct AssignStatement* assign_statement;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;

  line->type = ParserLineType_kAssignStatement;
  assign_statement = &line->variant.assign_statement;
  key_expr = &assign_statement->key_expr;
  key_expr->constexpr.expr = (char*)key;
  key_expr->constexpr.length = strlen(key);

  key_expr->subscripts = subscripts;
  for (key_expr->subscripts_count = 0;
      key_expr->subscripts_count < subscripts_count;
      ++key_expr->subscripts_count) {
    struct Subscript* current;

    current = &key_expr->subscripts[key_expr->subscripts_count];
    current->expr.expr = va_arg(args, char*);
    current->expr.length = strlen(current->expr.expr);
    current->expr.type = ConstExprType_kString;
  }

  value_expr = &assign_statement->value_expr;
  value_expr->type = ValueExprType_kConst;
  value_expr->variant.as_constexpr.expr = (char*)value;
  value_expr->variant.as_constexpr.length = strlen(value);
  value_expr->variant.as_constexpr.type = ConstExprType_kString;
}
