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

#include "bh/config/colonini/internal/parser/key_expr.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "bh/common/string_util/memstring.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/subscript.h"

static int Peek(
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    const struct LexerString** begin_token_src,
    const struct LexerString** end_token_src,
    int (*predicate)(const struct LexerString* token),
    size_t* error_column) {
  size_t dummy_error_column;
  const struct LexerString* first_token;
  const struct LexerString* last_token;

  if (error_column == NULL) {
    error_column = &dummy_error_column;
  }

  if (begin_src == NULL
      || end_src == NULL
      || begin_token_src == NULL
      || end_token_src == NULL
      || predicate == NULL) {
    *error_column = 1;
    return 0;
  }

  first_token = LexerString_CeilToken(begin_src);
  if (first_token == NULL) {
    *error_column = begin_src->line_index + 1;
    return 0;
  }

  if (!predicate(first_token)) {
    *error_column = first_token->line_index + 1;
    return 0;
  }

  for (last_token = first_token;
      last_token < end_src;
      last_token = last_token->next_token) {
    if (last_token == NULL) {
      *error_column = first_token->line_index + 1;
      return 0;
    }

    if (last_token->next_token == NULL
        || last_token->next_token >= end_src
        || !predicate(last_token->next_token)) {
      break;
    }
  }

  *begin_token_src = first_token;
  *end_token_src = &last_token[1];

  return 1;
}

static int IsValidPrimaryKeyToken(const struct LexerString* token) {
  static const char kValidChars[] =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  enum {
    kValidCharsLength = (sizeof(kValidChars) / sizeof(kValidChars[0])) - 1
  };

  size_t i_invalid;

  i_invalid =
      MemCSpn(token->str, token->str_length, kValidChars, kValidCharsLength);
  return (i_invalid == token->str_length);
}

/**
 * Starting from begin_src, parses the primary key constituted of
 * tokens and whitespace no further than end_src. Returns a non-NULL on
 * success, or else returns NULL and sets error column.
 */
static struct ConstExpr* ParsePrimary(
    struct ConstExpr* expr,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column) {
  size_t dummy_error_column;

  int peek_result;
  struct ConstExpr* init_primary_result;

  const struct LexerString* begin_key_src;
  const struct LexerString* end_key_src;

  if (error_column == NULL) {
    error_column = &dummy_error_column;
  }

  if (expr == NULL || begin_src == NULL || end_src == NULL) {
    *error_column = 1;
    goto error;
  }

  peek_result =
      Peek(
          begin_src,
          end_src,
          &begin_key_src,
          &end_key_src,
          &IsValidPrimaryKeyToken,
          error_column);
  if (!peek_result) {
    return NULL;
  }

  init_primary_result = ConstExpr_Init(expr, begin_key_src, end_key_src);
  if (init_primary_result == NULL) {
    *error_column = 1;
    return NULL;
  }
  /* Primary key is always interpretted as string. */
  expr->type = ConstExprType_kString;

  return expr;

error:
  return NULL;
}

/**
 * Starting from begin_src, parses the subkeys constituted of tokens
 * and whitespace no further than end_src. Returns a non-NULL on
 * success, or else returns NULL and sets error column.
 */
static struct KeyExpr* ParseSubkeys(
    struct KeyExpr* expr,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column) {
  size_t subscript_count;
  const struct LexerString* current;
  const struct LexerString* lbracket_src;
  const struct LexerString* rbracket_src;

  /* Determine the number of subkeys. */
  for (current = begin_src, subscript_count = 0;
      current != NULL && current < end_src;
      current = rbracket_src->next_token, ++subscript_count) {
    int peek_result;

    peek_result =
        Subscript_Peek(
            current, end_src, &lbracket_src, &rbracket_src, error_column);
    if (!peek_result) {
      if (Subscript_IsBegin(current)) {
        *error_column = current->line_index + 1;
        return NULL;
      }
      break;
    }
  }

  if (subscript_count == 0) {
    expr->subscripts = NULL;
    expr->subscript_count = 0;
    return expr;
  }

  /* Allocate space for the subkeys. */
  expr->subscripts =
      malloc(subscript_count * sizeof(expr->subscripts[0]));
  if (expr->subscripts == NULL) {
    *error_column = 1;
    goto error;
  }

  /* Parse the subkeys. */
  for (current = begin_src, expr->subscript_count = 0;
      expr->subscript_count < subscript_count;
      current = expr->subscripts[expr->subscript_count].end_src,
          ++expr->subscript_count) {
    struct Subscript* parse_result;

    parse_result =
        Subscript_Parse(
            &expr->subscripts[expr->subscript_count],
            current,
            end_src,
            error_column);
    if (parse_result == NULL) {
      return NULL;
    }
  }

  return expr;

error:
  return NULL;
}

/**
 * External
 */

struct KeyExpr* KeyExpr_Parse(
    struct KeyExpr* expr,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column) {
  size_t dummy_error_column;

  int peek_result;
  struct ConstExpr* parse_primary_result;
  struct KeyExpr* parse_subkeys_result;

  if (error_column == NULL) {
    error_column = &dummy_error_column;
  }

  if (expr == NULL || begin_src == NULL || end_src == NULL) {
    *error_column = 1;
    goto error;
  }

  parse_primary_result =
      ParsePrimary(&expr->primary, begin_src, end_src, error_column);
  if (parse_primary_result == NULL) {
    return NULL;
  }

  parse_subkeys_result =
      ParseSubkeys(expr, expr->primary.end_src, end_src, error_column);
  if (parse_subkeys_result == NULL) {
    return NULL;
  }

  return expr;

error:
  return NULL;
}

void KeyExpr_Deinit(struct KeyExpr* expr) {
  while (expr->subscript_count-- > 0) {
    Subscript_Deinit(&expr->subscripts[expr->subscript_count]);
  }

  ConstExpr_Deinit(&expr->primary);
}

int KeyExpr_CompareKeysAsStrings(
    const struct KeyExpr* left, const struct KeyExpr* right) {
  size_t i;
  int is_left_shorter;
  size_t max_count;

  int cmp_result;

  cmp_result =
      ConstExpr_CompareExprAsString(&left->primary, &right->primary);
  if (cmp_result != 0) {
    return cmp_result;
  }

  is_left_shorter = (left->subscript_count < right->subscript_count);
  max_count =
      is_left_shorter ? left->subscript_count : right->subscript_count;
  for (i = 0; i < max_count; ++i) {
    cmp_result =
        ConstExpr_CompareExprAsString(
            &left->subscripts[i].expr, &right->subscripts[i].expr);
    if (cmp_result != 0) {
      return cmp_result;
    }
  }

  if (left->subscript_count != right->subscript_count) {
    if (is_left_shorter) {
      return -1;
    } else {
      return 1;
    }
  }

  return 0;
}

int KeyExpr_Equal(
    const struct KeyExpr* left, const struct KeyExpr* right) {
  size_t i;

  if (!ConstExpr_Equal(&left->primary, &right->primary)) {
    return 0;
  }

  if (left->subscript_count != right->subscript_count) {
    return 0;
  }

  for (i = 0; i < left->subscript_count; ++i) {
    if (!Subscript_Equal(&left->subscripts[i], &right->subscripts[i])) {
      return 0;
    }
  }

  return 1;
}
