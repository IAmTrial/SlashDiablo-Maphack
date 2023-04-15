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
#include "bh/config/colonini/type.h"

static const char kDecimalDigits[] = "0123456789";
static const char kFalse[] = "false";
static const char kHexadecimalDigits[] = "0123456789ABCDEFabcdef";
static const char kTrue[] = "true";
enum {
  kDecimalDigitsLength =
      (sizeof(kDecimalDigits) / sizeof(kDecimalDigits[0])) - 1,
  kFalseLength = (sizeof(kFalse) / sizeof(kFalse[0])) - 1,
  kHexadecimalDigitsLength =
      (sizeof(kHexadecimalDigits) / sizeof(kHexadecimalDigits[0])) - 1,
  kTrueLength = (sizeof(kTrue) / sizeof(kTrue[0])) - 1
};

static const char* ConstExprType_GetDisplayName(enum ConstExprType type) {
  switch (type) {
    case ConstExprType_kInvalid: {
      static const char kDisplayName[] = "Invalid";
      return kDisplayName;
    }

    case ConstExprType_kString: {
      static const char kDisplayName[] = "String";
      return kDisplayName;
    }

    case ConstExprType_kSignedInt: {
      static const char kDisplayName[] = "SignedInt";
      return kDisplayName;
    }

    case ConstExprType_kUnsignedInt: {
      static const char kDisplayName[] = "UnsignedInt";
      return kDisplayName;
    }

    case ConstExprType_kBoolean: {
      static const char kDisplayName[] = "Boolean";
      return kDisplayName;
    }

    default: {
      assert(0 && "This should never happen.");
      return NULL;
    }
  }
}

static void ConstExpr_Deinit(struct ConstExpr* expr) {
  free(expr->expr);
  expr->expr = NULL;
  expr->length = 0;
  expr->end_src = NULL;
  expr->begin_src = NULL;
  expr->type = ConstExprType_kUnspecified;
}

static void ToggleExpr_Deinit(struct ToggleExpr* expr) {
  ConstExpr_Deinit(&expr->enabled_expr);
  ConstExpr_Deinit(&expr->input_expr);
}

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

static void ValueExpr_Deinit(struct ValueExpr* expr) {
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

static void AssignStatement_Deinit(struct AssignStatement* assign_statement) {
  ValueExpr_Deinit(&assign_statement->value_expr);
  KeyExpr_Deinit(&assign_statement->key_expr);
}

static enum ConstExprType GetConstExprType(const char* str, size_t length) {
  size_t i_invalid;
  enum ConstExprType type_candidate;

  if (length <= 0) {
    return 0;
  }

  /* Test true and false (boolean type). */
  if (length == kTrueLength || length == kFalseLength) {
    char temp[kFalseLength];
    size_t i;

    for (i = 0; i < length; ++i) {
      temp[i] = tolower(str[i]);
    }

    if (memcmp(temp, kFalse, kFalseLength) == 0
        || memcmp(temp, kTrue, kTrueLength) == 0) {
      return ConstExprType_kBoolean;
    }
  }

  /* Test decimal and hexadecimal (sint and uint types). */
  if (length >= 3 && str[0] == '0' && tolower(str[1]) == 'x') {
    type_candidate = ConstExprType_kUnsignedInt;
    i_invalid =
        MemCSpn(
            &str[2], length - 2, kHexadecimalDigits, kHexadecimalDigitsLength);
    /* Add 2 because if this is valid, then i_invalid would be length - 2. */
    i_invalid += 2;
  } else {
    type_candidate = ConstExprType_kSignedInt;
    i_invalid = MemCSpn(str, length, kDecimalDigits, kDecimalDigitsLength);
  }

  if (i_invalid == length) {
    return type_candidate;
  }

  return ConstExprType_kString;
}

static struct ConstExpr* ParsePrimaryKey(
    struct ConstExpr* key_expr,
    const struct LexerString* begin_lexer_str,
    size_t* error_column) {
  size_t i;

  size_t key_strs_count;
  size_t current_key_length;

  if (begin_lexer_str == NULL) {
    *error_column = 0;
    return NULL;
  }

  key_expr->begin_src = begin_lexer_str;

  /* Determine count of items everything up to the [ or : operator. */
  for (key_expr->end_src = key_expr->begin_src;
      ;
      key_expr->end_src = key_expr->end_src->next_token) {
    if (key_expr->end_src == NULL) {
      *error_column = 0;
      goto error;
    }
    assert(key_expr->end_src->str_length > 0);

    if (key_expr->end_src->str_length == 1
        && (key_expr->end_src->str[0] == '['
            || key_expr->end_src->str[0] == ':')) {
      key_expr->end_src = &key_expr->end_src->previous_token[1];
      break;
    }
  }
  /* Elements are contiguous, so this will give the index distance. */
  key_strs_count = key_expr->end_src - key_expr->begin_src;

  /* Determine the key string length. */
  key_expr->length = 0;
  for (i = 0; i < key_strs_count; ++i) {
    key_expr->length += key_expr->begin_src[i].str_length;
  }

  key_expr->expr = malloc((key_expr->length + 1) * sizeof(key_expr->expr[0]));
  if (key_expr->expr == NULL) {
    *error_column = 0;
    goto error;
  }

  /* Copy lexer key into parser's key. */
  current_key_length = 0;
  for (i = 0; i < key_strs_count; ++i) {
    memcpy(
        &key_expr->expr[current_key_length],
        key_expr->begin_src[i].str,
        key_expr->begin_src[i].str_length);
    current_key_length += key_expr->begin_src[i].str_length;
  }
  key_expr->expr[key_expr->length] = '\0';

  key_expr->type = GetConstExprType(key_expr->expr, key_expr->length);

  return key_expr;

error:
  return NULL;
}

static struct Subscript* ParseSubKey(
    struct Subscript* subscript,
    const struct LexerString* begin_lexer_str,
    size_t* error_column) {
  size_t i;

  size_t nest_level;
  size_t key_strs_count;
  size_t current_key_length;
  struct ConstExpr* key_expr;

  if (begin_lexer_str == NULL) {
    *error_column = 0;
    return NULL;
  }

  if (begin_lexer_str->str_length != 1
      || begin_lexer_str->str[0] != '[') {
    *error_column = begin_lexer_str->line_index;
    return NULL;
  }

  key_expr = &subscript->expr;
  key_expr->begin_src = begin_lexer_str->next_token;

  /* Determine count of items everything up to the matching ] operator. */
  nest_level = 1;
  for (key_expr->end_src = key_expr->begin_src;
      ;
      key_expr->end_src = key_expr->end_src->next_token) {
    if (key_expr->end_src == NULL) {
      *error_column = 0;
      return NULL;
    }
    assert(key_expr->end_src->str_length > 0);

    if (key_expr->end_src->str_length == 1) {
      if (key_expr->end_src->str[0] == '[') {
        ++nest_level;
      } else if (key_expr->end_src->str[0] == ']') {
        if (nest_level == 0) {
          *error_column = key_expr->end_src->line_index;
          return NULL;
        }

        --nest_level;
        if (nest_level == 0) {
          key_expr->end_src = &key_expr->end_src->previous_token[1];
          break;
        }
      }
    }
  }
  /* Elements are contiguous, so this will give the index distance. */
  key_strs_count = key_expr->end_src - key_expr->begin_src;

  /* Determine the key string length. */
  key_expr->length = 0;
  for (i = 0; i < key_strs_count; ++i) {
    key_expr->length += key_expr->begin_src[i].str_length;
  }

  key_expr->expr = malloc((key_expr->length + 1) * sizeof(key_expr->expr[0]));
  if (key_expr->expr == NULL) {
    *error_column = 0;
    goto error;
  }

  /* Copy lexer key into parser's key. */
  current_key_length = 0;
  for (i = 0; i < key_strs_count; ++i) {
    memcpy(
        &key_expr->expr[current_key_length],
        key_expr->begin_src[i].str,
        key_expr->begin_src[i].str_length);
    current_key_length += key_expr->begin_src[i].str_length;
  }
  key_expr->expr[key_expr->length] = '\0';

  key_expr->type = GetConstExprType(key_expr->expr, key_expr->length);

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

static struct ConstExpr* ParseValueAsConstExpr(
    struct ConstExpr* const_expr,
    const struct LexerString* begin_lexer_str,
    size_t* error_column) {
  size_t key_strs_count;
  size_t i;
  size_t current_value_length;

  if (begin_lexer_str == NULL) {
    *error_column = 0;
    return NULL;
  }

  const_expr->begin_src = begin_lexer_str;

  for (const_expr->end_src = const_expr->begin_src;
      const_expr->end_src->next_token != NULL;
      const_expr->end_src = const_expr->end_src->next_token) {}
  const_expr->end_src = &const_expr->end_src[1];

  /* Elements are contiguous, so this will give the index distance. */
  key_strs_count = const_expr->end_src - const_expr->begin_src;

  /* Determine the value length. */
  const_expr->length = 0;
  for (i = 0; i < key_strs_count; ++i) {
    const_expr->length += const_expr->begin_src[i].str_length;
  }

  const_expr->expr =
      malloc((const_expr->length + 1) * sizeof(const_expr->expr[0]));
  if (const_expr->expr == NULL) {
    *error_column = 0;
    goto error;
  }

  /* Copy lexer value into parser's value. */
  current_value_length = 0;
  for (i = 0; i < key_strs_count; ++i) {
    memcpy(
        &const_expr->expr[current_value_length],
        const_expr->begin_src[i].str,
        const_expr->begin_src[i].str_length);
    current_value_length += const_expr->begin_src[i].str_length;
  }
  const_expr->expr[const_expr->length] = '\0';

  const_expr->type = GetConstExprType(const_expr->expr, const_expr->length);

  return const_expr;

error:
  return NULL;
}

static struct ToggleExpr* ParseValueAsToggleExpr(
    struct ToggleExpr* toggle_expr,
    const struct LexerString* begin_lexer_str,
    size_t* error_column) {
  struct ConstExpr* input_expr;
  const struct LexerString* colon_op;
  struct ConstExpr* enabled_expr;

  if (begin_lexer_str == NULL) {
    *error_column = 0;
    return NULL;
  }

  /* Copy the first token into the enabled expression. */
  enabled_expr = &toggle_expr->enabled_expr;
  enabled_expr->begin_src = begin_lexer_str;
  enabled_expr->end_src = &enabled_expr->begin_src[1];
  enabled_expr->length = enabled_expr->begin_src->str_length;
  enabled_expr->expr =
      malloc((enabled_expr->length + 1) * sizeof(enabled_expr->expr[0]));
  if (enabled_expr->expr == NULL) {
    *error_column = 0;
    goto error;
  }
  memcpy(
      enabled_expr->expr, enabled_expr->begin_src->str, enabled_expr->length);
  enabled_expr->expr[enabled_expr->length] = '\0';
  enabled_expr->type =
      GetConstExprType(enabled_expr->expr, enabled_expr->length);

  /* Verify that the second token is the , operator. */
  colon_op = enabled_expr->end_src->previous_token->next_token;
  if (colon_op->str_length != 1 || colon_op->str[0] != ',') {
    *error_column = colon_op->line_index;
    return NULL;
  }

  /* Copy the third token into the input expression. */
  input_expr = &toggle_expr->input_expr;
  input_expr->begin_src = colon_op->next_token;
  input_expr->end_src = &input_expr->begin_src[1];
  input_expr->length = input_expr->begin_src->str_length;
  input_expr->expr =
      malloc((input_expr->length + 1) * sizeof(input_expr->expr[0]));
  if (input_expr->expr == NULL) {
    *error_column = 0;
    goto error;
  }
  memcpy(input_expr->expr, input_expr->begin_src->str, input_expr->length);
  input_expr->expr[input_expr->length] = '\0';
  input_expr->type = GetConstExprType(input_expr->expr, input_expr->length);

  return toggle_expr;

error:
  return NULL;
}

static struct ValueExpr* ParseValue(
    struct ValueExpr* value_expr,
    const struct LexerString* begin_lexer_str,
    size_t* error_column) {
  struct ConstExpr* parse_as_constexpr_result;
  struct ToggleExpr* parse_as_toggleexpr_result;

  const struct LexerString* second_lexer_str;
  const struct LexerString* third_lexer_str;
  enum ConstExprType first_token_type;
  enum ConstExprType third_token_type;

  /* If there is no value, assume it is of Empty type */
  if (begin_lexer_str == NULL) {
    value_expr->type = ValueExprType_kEmpty;
    return value_expr;
  }

  first_token_type =
      GetConstExprType(begin_lexer_str->str, begin_lexer_str->str_length);
  if (first_token_type != ConstExprType_kBoolean) {
    goto parse_value_as_constexpr;
  }

  second_lexer_str = begin_lexer_str->next_token;
  if (second_lexer_str == NULL
      || second_lexer_str->str_length != 1
      || second_lexer_str->str[0] != ',') {
    goto parse_value_as_constexpr;
  }

  third_lexer_str = second_lexer_str->next_token;
  if (third_lexer_str == NULL || third_lexer_str->next_token != NULL) {
    goto parse_value_as_constexpr;
  }

  third_token_type =
      GetConstExprType(third_lexer_str->str, third_lexer_str->str_length);
  if (third_token_type != ConstExprType_kString) {
    goto parse_value_as_constexpr;
  }

  value_expr->type = ValueExprType_kToggle;
  parse_as_toggleexpr_result =
      ParseValueAsToggleExpr(
          &value_expr->variant.as_toggleexpr, begin_lexer_str, error_column);
  if (parse_as_toggleexpr_result == NULL) {
    return NULL;
  }

  return value_expr;

parse_value_as_constexpr:
  value_expr->type = ValueExprType_kConst;
  parse_as_constexpr_result =
      ParseValueAsConstExpr(
          &value_expr->variant.as_constexpr, begin_lexer_str, error_column);
  if (parse_as_constexpr_result == NULL) {
    return NULL;
  }

  return value_expr;
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

  parse_value_result =
      ParseValue(
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
