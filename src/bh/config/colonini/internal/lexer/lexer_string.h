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

#ifndef BH_CONFIG_COLONINI_INTERNAL_LEXER_LEXER_STRING_H_
#define BH_CONFIG_COLONINI_INTERNAL_LEXER_LEXER_STRING_H_

#include <stddef.h>

#include "bh/config/colonini/type/string.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct LexerString {
  struct Colonini_String str;
  size_t line_index;

  /* Convenience members, members are still relatively contiguous. */
  struct LexerString* previous_token;
  struct LexerString* next_token;
};

/**
 * Initializes the string components of the LexerString.
 */
struct LexerString* LexerString_InitStrComponent(
    struct LexerString* lstr,
    const char* src,
    size_t length,
    size_t line_index);

/**
 * Deinitializes a LexerString, freeing up resources that were
 * allocated.
 */
void LexerString_Deinit(struct LexerString* lstr);

/**
 * Finds the ceiling token (ordered by linked-list chain) of a
 * LexerString and returns it, or else returns NULL.
 */
struct LexerString* LexerString_CeilToken(const struct LexerString* lstr);

/**
 * Finds the floor token (ordered by linked-list chain) of a
 * LexerString and returns it, or else returns NULL.
 */
struct LexerString* LexerString_FloorToken(const struct LexerString* lstr);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_LEXER_LEXER_STRING_H_ */
