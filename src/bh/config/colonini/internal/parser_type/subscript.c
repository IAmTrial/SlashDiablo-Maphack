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

#include "bh/config/colonini/internal/parser_type/subscript.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "bh/config/colonini/internal/lexer/lexer_string.h"
#include "bh/config/colonini/internal/parser_type/const_expr.h"

static const char kLBracket[] = "[";
static const char kRBracket[] = "]";

/**
 * External
 */

int Subscript_Peek(
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    const struct LexerString** lbracket_src,
    const struct LexerString** rbracket_src,
    size_t* error_column) {
  size_t dummy_error_column;
  const struct LexerString* current_src;
  const struct LexerString* temp_lbracket_src;
  size_t nest_level;

  if (error_column == NULL) {
    error_column = &dummy_error_column;
  }

  if (begin_src == NULL
      || end_src == NULL
      || lbracket_src == NULL
      || rbracket_src == NULL) {
    *error_column = 0;
    return 0;
  }

  /* Valid subscript consist of [ operator, constexpr, and ] operator. */
  if (begin_src->next_token == NULL) {
    *error_column = begin_src->line_index;
    return 0;
  }

  temp_lbracket_src = LexerString_CeilToken(begin_src);
  if (temp_lbracket_src->next_token == NULL
      || temp_lbracket_src->next_token->next_token == NULL) {
    *error_column = temp_lbracket_src->line_index;
    return 0;
  }
  
  /* The first token encountered must be the left bracket. */
  assert(temp_lbracket_src->str_length >= 1);
  if (temp_lbracket_src->str_length != 1
      || memcmp(temp_lbracket_src->str, kLBracket, sizeof(kLBracket)) != 0) {
    *error_column = temp_lbracket_src->line_index;
    return 0;
  }

  if (temp_lbracket_src >= end_src) {
    *error_column = temp_lbracket_src->line_index;
    return 0;
  }

  /* Find the matching right bracket. */
  nest_level = 1;
  for (current_src = temp_lbracket_src->next_token;
      current_src < end_src;
      current_src = current_src->next_token) {
    if (current_src == NULL) {
      *error_column = temp_lbracket_src->line_index;
      return 0;
    }

    /* Any string before the matching right bracket is copied. */
    assert(current_src->str_length >= 1);
    if (current_src->str_length != 1) {
      continue;
    }

    if (memcmp(current_src->str, kLBracket, sizeof(kLBracket)) == 0) {
      ++nest_level;
    } else if (memcmp(current_src->str, kRBracket, sizeof(kRBracket)) == 0) {
      if (nest_level <= 0) {
        *error_column = current_src->line_index;
        return 0;
      }
      --nest_level;

      /* Reached end of subscript (probably). */
      if (nest_level == 0) {
        break;
      }
    }
  }

  if (nest_level != 0 && current_src >= end_src) {
    *error_column = temp_lbracket_src->line_index;
    return 0;
  }

  *lbracket_src = temp_lbracket_src;
  *rbracket_src = current_src;

  return 1;
}

int Subscript_IsBegin(const struct LexerString* src) {
  const struct LexerString* ceil_src;

  if (src == NULL) {
    return 0;
  }

  ceil_src = LexerString_CeilToken(src);
  return (ceil_src->str_length == 1
      && memcmp(ceil_src->str, kLBracket, sizeof(kLBracket)) == 0);
}

struct Subscript* Subscript_Parse(
    struct Subscript* subscript,
    const struct LexerString* begin_src,
    const struct LexerString* end_src,
    size_t* error_column) {
  size_t dummy_error_column;
  int peek_result;
  struct ConstExpr* init_result;

  const struct LexerString* lbracket_src;
  const struct LexerString* rbracket_src;
  const struct LexerString* expr_begin_src;
  const struct LexerString* expr_end_src;

  if (error_column == NULL) {
    error_column = &dummy_error_column;
  }

  if (begin_src == NULL || end_src == NULL) {
    *error_column = 0;
    return NULL;
  }
  peek_result =
      Subscript_Peek(
          begin_src, end_src, &lbracket_src, &rbracket_src, error_column);
  if (!peek_result) {
    *error_column = begin_src->line_index;
    return NULL;
  }

  expr_begin_src = lbracket_src->next_token;
  expr_end_src = &rbracket_src->previous_token[1];

  init_result = ConstExpr_Init(&subscript->expr, expr_begin_src, expr_end_src);
  if (init_result == NULL) {
    *error_column = 0;
    goto error;
  }

  subscript->begin_src = lbracket_src;
  subscript->end_src = &rbracket_src[1];

  return subscript;

error:
  return NULL;
}

void Subscript_Deinit(struct Subscript* subscript) {
  ConstExpr_Deinit(&subscript->expr);
}
