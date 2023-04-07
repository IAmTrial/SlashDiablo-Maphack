/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2023  SlashDiablo Team
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

/**
 * BH
 * Copyright (C) 2011  McGod
 *
 * All rights reserved.
 */

#include "bh/config/colonini/internal/lexer.h"

#include <stddef.h>
#include <stdlib.h>

#include "bh/common/string_util/memstring.h"

static const char kOperators[] = ":[]";
static const char kWhitespaces[] = "\t\n\v\f\r ";
static const char kNonTokens[] = ":[]\t\n\v\f\r ";

enum {
  kOperatorsLength = (sizeof(kOperators) / sizeof(kOperators[0])) - 1,
  kWhitespacesLength = (sizeof(kWhitespaces) / sizeof(kWhitespaces[0])) - 1,
  kNonTokensLength = (sizeof(kNonTokens) / sizeof(kNonTokens[0])) - 1
};

static struct LexerString* LexToken(
    struct LexerString* lexer_str,
    const char* raw_line,
    size_t raw_line_length) {
  lexer_str->str_length =
      MemSpn(raw_line, raw_line_length, kNonTokens, kNonTokensLength);
  lexer_str->str =
      malloc((lexer_str->str_length + 1) * sizeof(lexer_str->str[0]));
  if (lexer_str->str == NULL) {
    goto error;
  }
  memcpy(lexer_str->str, raw_line, lexer_str->str_length);
  lexer_str->str[lexer_str->str_length] = '\0';

  return lexer_str;

error:
  return NULL;
}

static struct LexerString* LexOperator(
    struct LexerString* lexer_str,
    const char* raw_line,
    size_t raw_line_length) {
  lexer_str->str_length = 1;
  lexer_str->str =
      malloc((lexer_str->str_length + 1) * sizeof(lexer_str->str[0]));
  if (lexer_str->str == NULL) {
    goto error;
  }
  memcpy(lexer_str->str, raw_line, lexer_str->str_length);
  lexer_str->str[lexer_str->str_length] = '\0';

  return lexer_str;

error:
  return NULL;
}

static struct LexerString* LexWhitespace(
    struct LexerString* lexer_str,
    const char* raw_line,
    size_t raw_line_length) {
  lexer_str->str_length =
      MemCSpn(raw_line, raw_line_length, kWhitespaces, kWhitespacesLength);
  lexer_str->str =
      malloc((lexer_str->str_length + 1) * sizeof(lexer_str->str[0]));
  if (lexer_str->str == NULL) {
    goto error;
  }
  memcpy(lexer_str->str, raw_line, lexer_str->str_length);
  lexer_str->str[lexer_str->str_length] = '\0';

  return lexer_str;

error:
  return NULL;
}

/**
 * External
 */

/*
 * Allocation strategy should be as follows, to remove need to use
 * realloc to expand dynamic memory size:
 * - Allocate space for the worst-case line, where each token is
 *   immediately followed by whitespace. Number of elements is length
 *   of line.
 */

struct LexerLine* LexerLine_LexLine(
    struct LexerLine* line,
    size_t line_number,
    const char* raw_line,
    size_t raw_line_length) {
  void* realloc_result;
  size_t i;
  struct LexerString* last_token;

  line->line_number = line_number;
  line->strs = malloc(raw_line_length * sizeof(line->strs[0]));
  if (line->strs == NULL) {
    goto error;
  }
  line->strs_count = 0;

  last_token = NULL;
  for (i = 0;
      i < raw_line_length;
      i += line->strs[line->strs_count].str_length, ++line->strs_count) {
    struct LexerString* lex_result;
    size_t remaining_line_length;

    remaining_line_length = raw_line_length - i;
    switch (raw_line[i]) {
      case '\t':
      case '\n':
      case '\v':
      case '\f':
      case '\r':
      case ' ': {
        lex_result =
            LexWhitespace(
                &line->strs[line->strs_count],
                &raw_line[i],
                remaining_line_length);
        break;
      }

      case '[':
      case ']':
      case ':': {
        lex_result =
            LexOperator(
                &line->strs[line->strs_count],
                &raw_line[i],
                remaining_line_length);
        break;
      }

      default: {
        lex_result =
            LexToken(
                &line->strs[line->strs_count],
                &raw_line[i],
                remaining_line_length);
        if (lex_result != NULL) {
          line->strs[line->strs_count].previous_token = last_token;
          if (last_token != NULL) {
            last_token->next_token = &line->strs[line->strs_count];
          }

          last_token = &line->strs[line->strs_count];
        }

        // TODO: Implement linking to the next token
        break;
      }
    }

    if (lex_result == NULL) {
      goto free_line_strs;
    }
  }

  /* Reduce the size, now that lexing is completed. */
  realloc_result =
      realloc(line->strs, line->strs_count * sizeof(line->strs[0]));
  if (realloc_result == NULL) {
    goto free_line_strs;
  }
  line->strs = realloc_result;

  return line;

free_line_strs:
  LexerLine_Deinit(line);

error:
  return NULL;
}

void LexerLine_Deinit(struct LexerLine* line) {
  size_t i;

  for (i = line->strs_count; i-- > 0; ) {
    line->strs[i].str_length = 0;
    free(line->strs[i].str);
    line->strs[i].str = NULL;
  }
  line->strs_count = 0;
  free(line->strs);
  line->strs = NULL;
}
