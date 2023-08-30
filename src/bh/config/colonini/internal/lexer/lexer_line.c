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

#include "bh/config/colonini/internal/lexer/lexer_line.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "bh/common/string_util/memstring.h"
#include "bh/config/colonini/internal/lexer/lexer_string.h"

static const char kOperators[] = ":[],";
static const char kWhitespaces[] = "\t\n\v\f\r ";
static const char kNonTokens[] = ":[],\t\n\v\f\r ";
static const char kSingleLineCommentStart[] = "//";

enum {
  kOperatorsLength = (sizeof(kOperators) / sizeof(kOperators[0])) - 1,
  kWhitespacesLength = (sizeof(kWhitespaces) / sizeof(kWhitespaces[0])) - 1,
  kNonTokensLength = (sizeof(kNonTokens) / sizeof(kNonTokens[0])) - 1,
  kSingleLineCommentStartLength =
      (sizeof(kSingleLineCommentStart) / sizeof(kSingleLineCommentStart[0]))
          - 1
};

enum CaptureRuleCategory {
  CaptureRuleCategory_kUnspecified,

  CaptureRuleCategory_kIdentifier,
  CaptureRuleCategory_kOperator,
  CaptureRuleCategory_kWhitespace,
  CaptureRuleCategory_kSingleLineComment
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
 * - Single line comment: Captures all remaining characters.
 */
static const struct CaptureRule kCaptureRulesSortedTable[] = {
  { CaptureRuleCategory_kIdentifier, &MemSpn, kNonTokens, kNonTokensLength },
  { CaptureRuleCategory_kOperator, &CaptureOperator, kOperators, kOperatorsLength },
  { CaptureRuleCategory_kWhitespace, &MemCSpn, kWhitespaces, kWhitespacesLength },
  { CaptureRuleCategory_kSingleLineComment, &MemSpn, "", 0 }
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

static enum CaptureRuleCategory GetCaptureRuleCategory(
    const char* str, size_t length) {
  switch (str[0]) {
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
    case ':':
    case ',': {
      return CaptureRuleCategory_kOperator;
    }

    case '/': {
      if (length > 1 && str[1] == '/') {
        return CaptureRuleCategory_kSingleLineComment;
      }
      return CaptureRuleCategory_kIdentifier;
    }

    default: {
      return CaptureRuleCategory_kIdentifier;
    }
  }
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
  size_t reserved_str_count;

  line->line_number = line_number;

  if (raw_line_length == 0) {
    line->strs = NULL;
    line->str_count = 0;
    line->token_count = 0;
    line->first_token = NULL;
    line->last_token = NULL;

    return line;
  }

  /* Determine the number of LexerString to reserve. */
  for (i_raw_line = 0, reserved_str_count = 0;
      i_raw_line < raw_line_length;
      i_raw_line += capture_length, ++reserved_str_count) {
    enum CaptureRuleCategory category;
    const struct CaptureRule* rule;
    size_t remaining_line_length;

    /* Read the current character to determine which capture rule to apply. */
    remaining_line_length = raw_line_length - i_raw_line;
    category =
        GetCaptureRuleCategory(&raw_line[i_raw_line], remaining_line_length);
    assert(category != CaptureRuleCategory_kUnspecified);

    rule = SearchCaptureRuleTable(category);
    if (rule == NULL) {
      goto error;
    }

    capture_length =
        rule->capture_func(
            &raw_line[i_raw_line],
            remaining_line_length,
            rule->capture_str,
            rule->capture_str_length);
    assert(capture_length > 0);
  }

  line->strs = malloc(reserved_str_count * sizeof(line->strs[0]));
  if (line->strs == NULL) {
    goto error;
  }

  /* Lex the line into individual LexerString. */
  line->str_count = 0;
  line->token_count = 0;
  line->last_token = NULL;
  for (i_raw_line = 0;
      i_raw_line < raw_line_length;
      i_raw_line += line->strs[line->str_count].str_length,
          ++line->str_count) {
    struct LexerString* init_str_component_result;
    struct LexerString* current_str;
    size_t str_length;
    enum CaptureRuleCategory category;
    const struct CaptureRule* rule;
    size_t remaining_line_length;

    current_str = &line->strs[line->str_count];
    current_str->previous_token = line->last_token;

    remaining_line_length = raw_line_length - i_raw_line;
    category =
        GetCaptureRuleCategory(&raw_line[i_raw_line], remaining_line_length);
    rule = SearchCaptureRuleTable(category);
    if (rule == NULL) {
      goto error_deinit_line;
    }

    str_length =
        rule->capture_func(
            &raw_line[i_raw_line],
            remaining_line_length,
            rule->capture_str,
            rule->capture_str_length);
    init_str_component_result =
        LexerString_InitStrComponent(
            current_str, &raw_line[i_raw_line], str_length, i_raw_line);
    if (init_str_component_result == NULL) {
      goto error_deinit_line;
    }

    if (category == CaptureRuleCategory_kIdentifier
        || category == CaptureRuleCategory_kOperator) {
      /* Set the next token for all previous strings. */
      for (i_strs =
              (line->last_token == NULL)
                  ? 0
                  : line->last_token - line->strs;
          i_strs < line->str_count;
          ++i_strs) {
        line->strs[i_strs].next_token = current_str;
      }

      line->last_token = current_str;
      ++line->token_count;
    }
  }

  /* Set the ending LexerString to have no token. */
  for (i_strs = (line->last_token == NULL) ? 0 : line->last_token - line->strs;
      i_strs < line->str_count;
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

  for (i = line->str_count; i-- > 0; ) {
    LexerString_Deinit(&line->strs[i]);
  }
  line->str_count = 0;
  free(line->strs);
  line->strs = NULL;
  line->first_token = NULL;
  line->last_token = NULL;
  line->token_count = 0;
}
