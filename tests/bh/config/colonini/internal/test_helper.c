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
