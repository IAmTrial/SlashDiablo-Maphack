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
#include "bh/config/colonini/internal/parser_type/const_expr.h"
#include "bh/config/colonini/internal/parser_type/toggle_expr.h"
#include "bh/config/colonini/internal/parser_type/value_expr.h"
#include "bh/config/colonini/type.h"

static void Subscript_Deinit(struct Subscript* subscript) {
  ConstExpr_Deinit(&subscript->expr);
}

static void KeyExpr_Deinit(struct KeyExpr* expr) {
  size_t i;

  for (i = expr->subscripts_count; i-- > 0; ) {
    Subscript_Deinit(&expr->subscripts[i]);
  }

  ConstExpr_Deinit(&expr->constexpr);
}

static void AssignStatement_Deinit(struct AssignStatement* assign_statement) {
  ValueExpr_Deinit(&assign_statement->value_expr);
  KeyExpr_Deinit(&assign_statement->key_expr);
}

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

static struct Subscript* ParseSubKey(
    struct Subscript* subscript,
    const struct LexerString* begin_src,
    size_t* error_column) {
  struct ConstExpr* init_result;

  const struct LexerString* current_src;
  const struct LexerString* end_src;

  size_t nest_level;

  if (begin_src == NULL) {
    *error_column = 0;
    return NULL;
  }

  if (begin_src->str_length != 1 || begin_src->str[0] != '[') {
    *error_column = begin_src->line_index;
    return NULL;
  }

  /* Find the last string before the matching ] operator. */
  nest_level = 0;
  for (current_src = begin_src; ; current_src = current_src->next_token) {
    if (current_src == NULL) {
      *error_column = 0;
      return NULL;
    }
    assert(current_src->str_length > 0);

    if (current_src->str_length == 1) {
      if (current_src->str[0] == '[') {
        ++nest_level;
      } else if (current_src->str[0] == ']') {
        if (nest_level == 0) {
          *error_column = current_src->line_index;
          return NULL;
        }

        --nest_level;
        if (nest_level == 0) {
          end_src = &current_src->previous_token[1];
          break;
        }
      }
    }
  }

  if (begin_src == end_src) {
    *error_column = 0;
    return NULL;
  }

  init_result =
      ConstExpr_Init(&subscript->expr, begin_src->next_token, end_src);
  if (init_result == NULL) {
    *error_column = 0;
    goto error;
  }

  return subscript;

error:
  return NULL;
}

static struct KeyExpr* ParseSubKeys(
    struct KeyExpr* key_expr,
    const struct LexerString* begin_lexer_str,
    size_t* error_column) {
  const struct LexerString* current;
  size_t nest_level;
  size_t i;

  if (begin_lexer_str == NULL) {
    *error_column = 0;
    return NULL;
  }

  /*
   * Determine the number of sub keys. Brackets can be nested in
   * subkeys, so they need to be accounted for.
   */
  nest_level = 0;
  key_expr->subscripts_count = 0;
  for (current = begin_lexer_str; ; current = current->next_token) {
    assert(current->str_length > 0);
    if (current->str_length == 1) {
      if (current->str[0] == '[') {
        ++nest_level;
      } else if (current->str[0] == ']') {
        if (nest_level == 0) {
          *error_column = current->line_index;
          return NULL;
        }

        --nest_level;
        if (nest_level == 0) {
          ++key_expr->subscripts_count;
        }
      } else if (current->str[0] == ':') {
        /* Colon means that the key parsing should end. */
        if (nest_level != 0) {
          *error_column = current->line_index;
          return NULL;
        }
        break;
      }
    } else if (nest_level == 0) {
      /*
       * There should not be any string tokens outside of brackets at
       * the subkey parsing state.
       */
      assert(current->str_length >= 2);
      *error_column = 0;
      return NULL;
    }
  }

  /* Allocate space for the subkeys. */
  key_expr->subscripts =
      malloc(key_expr->subscripts_count * sizeof(key_expr->subscripts[0]));
  if (key_expr->subscripts == NULL) {
    *error_column = 0;
    goto error;
  }

  /* Parse the subkeys. */
  for (current = begin_lexer_str, i = 0;
      i < key_expr->subscripts_count;
      current = key_expr->subscripts[i].expr.end_src->previous_token->next_token->next_token, ++i) {
    struct Subscript* parse_sub_key_result;

    parse_sub_key_result =
        ParseSubKey(
            &key_expr->subscripts[i], current, error_column);
    if (parse_sub_key_result == NULL) {
      return NULL;
    }
  }

  return key_expr;

error:
  return NULL;
}

static struct KeyExpr* ParseKeys(
    struct KeyExpr* key_expr,
    const struct LexerString* begin_lexer_str,
    size_t* error_column) {
  struct ConstExpr* parse_primary_key_result;
  struct KeyExpr* parse_sub_keys_result;
  const struct LexerString* subscript_begin;

  if (begin_lexer_str == NULL) {
    *error_column = 0;
    return NULL;
  }

  parse_primary_key_result =
      ParsePrimaryKey(&key_expr->constexpr, begin_lexer_str, error_column);
  if (parse_primary_key_result == NULL) {
    return NULL;
  }

  subscript_begin = key_expr->constexpr.end_src->previous_token->next_token;
  if (subscript_begin->str_length != 1 || subscript_begin->str[0] != '[') {
    key_expr->subscripts_count = 0;
    return key_expr;
  }

  parse_sub_keys_result =
      ParseSubKeys(key_expr, subscript_begin, error_column);
  if (parse_sub_keys_result == NULL) {
    return NULL;
  }

  return key_expr;
}

static struct AssignStatement* ParseAssignStatement(
    struct AssignStatement* assign_statement,
    const struct LexerString* begin_lexer_str,
    size_t* error_column) {
  struct KeyExpr* parse_keys_result;
  struct ValueExpr* parse_value_result;

  struct KeyExpr* key_expr;
  size_t subscript_count;
  struct LexerString* key_end_src;
  const struct LexerString* colon_op;
  struct ValueExpr* value_expr;

  if (begin_lexer_str == NULL) {
    *error_column = 0;
    return NULL;
  }

  key_expr = &assign_statement->key_expr;

  parse_keys_result =
      ParseKeys(
          &assign_statement->key_expr,
          begin_lexer_str,
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

  if (!ValueExpr_IsValid(colon_op->next_token, error_column)) {
    return NULL;
  }
  parse_value_result =
      ValueExpr_Parse(
          &assign_statement->value_expr,
          colon_op->next_token,
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
