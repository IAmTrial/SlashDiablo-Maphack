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

#ifndef BH_CONFIG_COLONINI_INTERNAL_LEXER_H_
#define BH_CONFIG_COLONINI_INTERNAL_LEXER_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct LexerString {
  char* str;
  size_t str_length;
  size_t line_index;

  /* Convenience members, members are still relatively contiguous. */
  struct LexerString* previous_token;
  struct LexerString* next_token;
};

struct LexerLine {
  size_t line_number;
  struct LexerString* strs;
  size_t strs_count;

  struct LexerString* first_token;
  struct LexerString* last_token;
  size_t tokens_count;
};

/**
 * Lexes a line of the specified length into separated tokens and
 * whitespaces.
 */
struct LexerLine* LexerLine_LexLine(
    struct LexerLine* line,
    size_t line_number,
    const char* raw_line,
    size_t raw_line_length);

/**
 * Deinitializes a LexerLine, freeing up resources that were allocated.
*/
void LexerLine_Deinit(struct LexerLine* line);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_LEXER_H_ */
