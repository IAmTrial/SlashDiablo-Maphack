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

#include <assert.h>
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

enum CaptureRuleCategory {
  CaptureRuleCategory_kUnspecified,

  CaptureRuleCategory_kIdentifier,
  CaptureRuleCategory_kOperator,
  CaptureRuleCategory_kWhitespace
};

struct CaptureRule {
  enum CaptureRuleCategory category;
  size_t (*capture_func)(
      const char* data,
      size_t data_size,
      const char* capture,
      size_t capture_size);
  const char* capture_str;
  size_t capture_str_length;
};

static int CaptureRule_CompareCategory(
    const struct CaptureRule* first, const struct CaptureRule* second) {
  return first->category - second->category;
}

static int CaptureRule_CompareCategoryAsVoid(
    const void* first, const void* second) {
  return CaptureRule_CompareCategory(first, second);
}

static size_t CaptureOperator(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size) {
  /* The size can be expanded if operators of length 2 or more are needed. */
  return 1;
}

/**
 * Capture table:
 * - Identifier: Captures contiguous characters not considered
 *   whitespace or operator.
 * - Operator: Captures characters, whose length is dependent on the
 *   operator. For example, ] folowed by [ are considered separate
 *   tokens.
 * - Whitespace: Captures contiguous whitespace characters. Required
 *   for writing to the config with the same spacing as the input.
 */
static const struct CaptureRule kCaptureRulesSortedTable[] = {
  { CaptureRuleCategory_kIdentifier, &MemSpn, kNonTokens, kNonTokensLength },
  { CaptureRuleCategory_kOperator, &CaptureOperator, kOperators, kOperatorsLength },
  { CaptureRuleCategory_kWhitespace, &MemCSpn, kWhitespaces, kWhitespacesLength },
};

enum {
  kCaptureRulesSortedTableCount =
      sizeof(kCaptureRulesSortedTable) / sizeof(kCaptureRulesSortedTable[0])
};

static const struct CaptureRule* SearchCaptureRuleTable(
    enum CaptureRuleCategory category) {
  struct CaptureRule search_key;
  const struct CaptureRule* search_result;

  search_key.category = category;
  search_result =
      bsearch(
          &search_key,
          kCaptureRulesSortedTable,
          kCaptureRulesSortedTableCount,
          sizeof(kCaptureRulesSortedTable[0]),
          CaptureRule_CompareCategoryAsVoid);
  return search_result;
}

static enum CaptureRuleCategory GetCaptureRuleCategory(char ch) {
  switch (ch) {
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r':
    case ' ': {
      return CaptureRuleCategory_kWhitespace;
    }

    case '[':
    case ']':
    case ':': {
      return CaptureRuleCategory_kOperator;
    }

    default: {
      return CaptureRuleCategory_kIdentifier;
    }
  }
}

static void LexerString_Deinit(struct LexerString* lexer_str) {
  lexer_str->next_token = NULL;
  lexer_str->previous_token = NULL;
  lexer_str->str_length = 0;
  free(lexer_str->str);
  lexer_str->str = NULL;
}

/**
 * External
 */

struct LexerLine* LexerLine_LexLine(
    struct LexerLine* line,
    size_t line_number,
    const char* raw_line,
    size_t raw_line_length) {
  size_t i_raw_line;
  size_t i_strs;
  size_t capture_length;
  size_t reserved_strs_count;

  line->line_number = line_number;

  if (raw_line_length == 0) {
    line->strs = NULL;
    line->strs_count = 0;
    line->tokens_count = 0;
    line->first_token = NULL;
    line->last_token = NULL;

    return line;
  }

  /* Determine the number of LexerString to reserve. */
  for (i_raw_line = 0, reserved_strs_count = 0;
      i_raw_line < raw_line_length;
      i_raw_line += capture_length, ++reserved_strs_count) {
    enum CaptureRuleCategory category;
    const struct CaptureRule* rule;
    size_t remaining_line_length;

    /* Read the current character to determine which capture rule to apply. */
    category = GetCaptureRuleCategory(raw_line[i_raw_line]);
    assert(category != CaptureRuleCategory_kUnspecified);

    rule = SearchCaptureRuleTable(category);
    if (rule == NULL) {
      goto error;
    }

    remaining_line_length = raw_line_length - i_raw_line;
    capture_length =
        rule->capture_func(
            &raw_line[i_raw_line],
            remaining_line_length,
            rule->capture_str,
            rule->capture_str_length);
    assert(capture_length > 0);
  }

  line->strs = malloc(reserved_strs_count * sizeof(line->strs[0]));
  if (line->strs == NULL) {
    goto error;
  }

  /* Lex the line into individual LexerString. */
  line->strs_count = 0;
  line->tokens_count = 0;
  line->last_token = NULL;
  for (i_raw_line = 0;
      i_raw_line < raw_line_length;
      i_raw_line += line->strs[line->strs_count].str_length,
          ++line->strs_count) {
    struct LexerString* current_str;
    enum CaptureRuleCategory category;
    const struct CaptureRule* rule;
    size_t remaining_line_length;

    current_str = &line->strs[line->strs_count];
    current_str->previous_token = line->last_token;

    category = GetCaptureRuleCategory(raw_line[i_raw_line]);
    rule = SearchCaptureRuleTable(category);
    if (rule == NULL) {
      goto error_deinit_line;
    }

    remaining_line_length = raw_line_length - i_raw_line;
    current_str->str_length =
        rule->capture_func(
            &raw_line[i_raw_line],
            remaining_line_length,
            rule->capture_str,
            rule->capture_str_length);
    current_str->str =
        malloc((current_str->str_length + 1) * sizeof(line->strs[0].str[0]));
    if (current_str->str == NULL) {
      goto error_deinit_line;
    }
    memcpy(current_str->str, &raw_line[i_raw_line], current_str->str_length);
    current_str->str[current_str->str_length] = '\0';

    if (category == CaptureRuleCategory_kIdentifier
        || category == CaptureRuleCategory_kOperator) {
      /* Set the next token for all previous strings. */
      for (i_strs =
              (line->last_token == NULL)
                  ? 0
                  : line->last_token - line->strs;
          i_strs < line->strs_count;
          ++i_strs) {
        line->strs[i_strs].next_token = current_str;
      }

      line->last_token = current_str;
      ++line->tokens_count;
    }
  }

  /* Set the ending LexerString to have no token. */
  for (i_strs = (line->last_token == NULL) ? 0 : line->last_token - line->strs;
      i_strs < line->strs_count;
      ++i_strs) {
    line->strs[i_strs].next_token = NULL;
  }

  /* Set the first token. */
  if (line->last_token == NULL) {
    line->first_token = NULL;
  } else if (line->strs[0].next_token == NULL) {
    line->first_token = line->last_token;
  } else if (line->strs[0].next_token->previous_token == NULL) {
    line->first_token = line->strs[0].next_token;
  } else {
    line->first_token = line->strs[0].next_token->previous_token;
  }

  return line;

error_deinit_line:
  LexerLine_Deinit(line);

error:
  return NULL;
}

void LexerLine_Deinit(struct LexerLine* line) {
  size_t i;

  for (i = line->strs_count; i-- > 0; ) {
    LexerString_Deinit(&line->strs[i]);
  }
  line->strs_count = 0;
  free(line->strs);
  line->strs = NULL;
  line->first_token = NULL;
  line->last_token = NULL;
  line->tokens_count = 0;
}
