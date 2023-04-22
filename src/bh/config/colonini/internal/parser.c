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

#include "bh/config/colonini/internal/parser.h"

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "bh/common/string_util/memstring.h"
#include "bh/config/colonini/internal/lexer.h"
#include "bh/config/colonini/internal/lexer/lexer_string.h"
#include "bh/config/colonini/internal/parser_type/assign_statement.h"
#include "bh/config/colonini/internal/parser_type/const_expr.h"
#include "bh/config/colonini/internal/parser_type/key_expr.h"
#include "bh/config/colonini/internal/parser_type/subscript.h"
#include "bh/config/colonini/internal/parser_type/toggle_expr.h"
#include "bh/config/colonini/internal/parser_type/value_expr.h"
#include "bh/config/colonini/type.h"

static struct AssignStatement* ParseAssignStatement(
    struct AssignStatement* assign_statement,
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
    *error_column = 0;
    return NULL;
  }

  first_token = LexerString_CeilToken(begin_src);
  if (first_token == NULL || first_token >= end_src) {
    *error_column = begin_src->line_index;
    return NULL;
  }

  key = &assign_statement->key_expr;
  parse_key_result = KeyExpr_Parse(key, begin_src, end_src, error_column);
  if (parse_key_result == NULL) {
    return NULL;
  }

  key_end_src =
      (key->subscripts_count == 0)
          ? key->constexpr.end_src
          : key->subscripts[key->subscripts_count - 1].end_src;
  if (key_end_src == NULL || key_end_src >= end_src) {
    *error_column = first_token->line_index;
    return NULL;
  }

  colon_op = LexerString_CeilToken(key_end_src);
  if (colon_op == NULL || colon_op >= end_src) {
    return NULL;
  }

  if (colon_op->str_length != 1
      || memcmp(colon_op->str, ":", sizeof(":")) != 0) {
    *error_column = colon_op->line_index;
    return NULL;
  }

  value_begin_src = colon_op->next_token;
  parse_value_result =
      ValueExpr_Parse(
          &assign_statement->value_expr,
          value_begin_src,
          end_src,
          error_column);
  if (parse_value_result == NULL) {
    return NULL;
  }

  return assign_statement;
}

/*
 * Parse rules:
 * S -> K : V
 *
 * K -> sP
 * P -> AP
 * A -> [d]
 * A -> [s]
 * P -> e
 * B -> e
 * V -> R
 * R -> bT
 * T -> , s
 * T -> e
 * R -> s
 * V -> d
 * V -> h
 *
 * Terminals (regex):
 * e(mpty) ->
 * b(oolean) -> (true)|(false)
 * d(ecimal) -> (0|([1-9][0-9]{0,9}))
 * h(exadecimal) -> 0(x|X)[0-9A-Fa-f]{1,7}
 * s(tring) -> [^\t\n\v\f\r ][^\[\]:]*[^\[\]:\t\n\v\f\r ]
 */

/**
 * External
 */

struct ParserLine* ParserLine_ParseLine(
    struct ParserLine* parser_line,
    const struct LexerLine* lexer_line,
    size_t* error_column) {
  struct AssignStatement* parse_assign_statement_result;

  parser_line->line_number = lexer_line->line_number;

  if (lexer_line->tokens_count == 0) {
    parser_line->type = ParserLineType_kNoOp;
    return parser_line;
  }

  parse_assign_statement_result =
      ParseAssignStatement(
          &parser_line->variant.assign_statement,
          lexer_line->first_token,
          &lexer_line->last_token[1],
          error_column);
  if (parse_assign_statement_result == NULL) {
    parser_line->type = ParserLineType_kInvalid;
    return NULL;
  }

  parser_line->type = ParserLineType_kAssignStatement;

  return parser_line;
}

void ParserLine_Deinit(struct ParserLine* parser_line) {
  switch (parser_line->type) {
    case ParserLineType_kNoOp:
    case ParserLineType_kInvalid: {
      break;
    }

    case ParserLineType_kAssignStatement: {
      AssignStatement_Deinit(&parser_line->variant.assign_statement);
      break;
    }

    default: {
      assert(0 && "This should never happen.");
      return;
    }
  }
  parser_line->line_number = 0;
}
