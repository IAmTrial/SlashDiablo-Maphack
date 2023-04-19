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

static struct ConstExpr* ParsePrimaryKey(
    struct ConstExpr* key_expr,
    const struct LexerString* begin_src,
    size_t* error_column) {
  struct ConstExpr* init_result;

  const struct LexerString* current_src;
  const struct LexerString* end_src;

  /* Find the last string before the [ or : operator. */
  for (current_src = begin_src; ; current_src = current_src->next_token) {
    if (current_src == NULL) {
      *error_column = 0;
      return NULL;
    }
    assert(current_src->str_length > 0);

    if (current_src->str_length == 1
        && (current_src->str[0] == '[' || current_src->str[0] == ':')) {
      end_src = &current_src->previous_token[1];
      break;
    }
  }

  if (begin_src == end_src) {
    *error_column = 0;
    return NULL;
  }

  init_result = ConstExpr_Init(key_expr, begin_src, end_src);
  if (init_result == NULL) {
    *error_column = 0;
    goto error;
  }

  return key_expr;

error:
  return NULL;
}

static struct KeyExpr* ParseSubKeys(
    struct KeyExpr* key_expr,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column) {
  size_t subscripts_count;
  const struct LexerString* current;
  const struct LexerString* lbracket_src;
  const struct LexerString* rbracket_src;

  /* Determine the number of subkeys. */
  for (current = begin_src, subscripts_count = 0;
      current != end_src;
      current = &rbracket_src[1], ++subscripts_count) {
    int peek_result;

    peek_result =
        Subscript_Peek(
            current, end_src, &lbracket_src, &rbracket_src, error_column);
    if (!peek_result) {
      if (Subscript_IsBegin(current)) {
        *error_column = current->line_index;
        return NULL;
      }
      break;
    }
  }

  if (subscripts_count == 0) {
    key_expr->subscripts_count = 0;
    return key_expr;
  }

  /* Allocate space for the subkeys. */
  key_expr->subscripts =
      malloc(subscripts_count * sizeof(key_expr->subscripts[0]));
  if (key_expr->subscripts == NULL) {
    *error_column = 0;
    goto error;
  }

  /* Parse the subkeys. */
  for (current = begin_src, key_expr->subscripts_count = 0;
      current != end_src && key_expr->subscripts_count < subscripts_count;
      current = &rbracket_src[1], ++key_expr->subscripts_count) {
    int peek_result;
    struct Subscript* parse_result;

    peek_result =
        Subscript_Peek(
            current, end_src, &lbracket_src, &rbracket_src, error_column);
    assert(peek_result);
    parse_result =
        Subscript_Parse(
            &key_expr->subscripts[key_expr->subscripts_count],
            lbracket_src,
            &rbracket_src[1],
            error_column);
    if (parse_result == NULL) {
      return NULL;
    }
  }

  return key_expr;

error:
  return NULL;
}

static struct KeyExpr* ParseKeys(
    struct KeyExpr* key_expr,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column) {
  struct ConstExpr* parse_primary_key_result;
  struct KeyExpr* parse_sub_keys_result;
  const struct LexerString* subscript_begin;

  if (begin_src == NULL) {
    *error_column = 0;
    return NULL;
  }

  parse_primary_key_result =
      ParsePrimaryKey(&key_expr->constexpr, begin_src, error_column);
  if (parse_primary_key_result == NULL) {
    return NULL;
  }

  subscript_begin = key_expr->constexpr.end_src->previous_token->next_token;
  if (subscript_begin->str_length != 1 || subscript_begin->str[0] != '[') {
    key_expr->subscripts_count = 0;
    return key_expr;
  }

  parse_sub_keys_result =
      ParseSubKeys(key_expr, subscript_begin, end_src, error_column);
  if (parse_sub_keys_result == NULL) {
    return NULL;
  }

  return key_expr;
}

static struct AssignStatement* ParseAssignStatement(
    struct AssignStatement* assign_statement,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column) {
  struct KeyExpr* parse_keys_result;
  struct ValueExpr* parse_value_result;

  struct KeyExpr* key_expr;
  size_t subscript_count;
  struct LexerString* key_end_src;
  const struct LexerString* colon_op;
  const struct LexerString* value_begin_src;
  const struct LexerString* value_end_src;
  struct ValueExpr* value_expr;

  if (begin_src == NULL) {
    *error_column = 0;
    return NULL;
  }

  key_expr = &assign_statement->key_expr;

  parse_keys_result =
      ParseKeys(
          &assign_statement->key_expr,
          begin_src,
          end_src,
          error_column);
  if (parse_keys_result == NULL) {
    return NULL;
  }

  if (key_expr->subscripts_count <= 0) {
    colon_op = key_expr->constexpr.end_src->previous_token->next_token;
  } else {
    struct Subscript* last_subscript;

    last_subscript = &key_expr->subscripts[key_expr->subscripts_count - 1];
    colon_op =
        last_subscript->expr.end_src->previous_token->next_token->next_token;
  }

  if (colon_op == NULL) {
    *error_column = 0;
    return NULL;
  }

  if (colon_op->str_length != 1 || colon_op->str[0] != ':') {
    *error_column = colon_op->line_index;
    return NULL;
  }

  value_begin_src = colon_op->next_token;
  for (value_end_src = value_begin_src;
      value_end_src != NULL && value_end_src->next_token != NULL;
      value_end_src = value_end_src->next_token) {}
  if (value_begin_src != NULL) {
    ++value_end_src;
  }
  if (!ValueExpr_IsValid(value_begin_src, value_end_src, error_column)) {
    return NULL;
  }
  parse_value_result =
      ValueExpr_Parse(
          &assign_statement->value_expr,
          value_begin_src,
          value_end_src,
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
