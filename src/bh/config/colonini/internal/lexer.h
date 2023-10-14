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

#include "bh/config/colonini/internal/lexer/lexer_line.h"
#include "bh/config/colonini/type/string.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct Lexer {
  size_t count;
  struct LexerLine* lines;
};

/**
 * Initializes a Lexer with space allocated for the specified number
 * of LexerLine.
 */
struct Lexer* Lexer_Init(struct Lexer* lexer, size_t line_count);

/**
 * Deinitializes a Lexer, freeing up resources that were allocated.
 */
void Lexer_Deinit(struct Lexer* lexer);

/**
 * Lexes a specified contiguous number of null-terminated, UTF-8
 * encoded lines. Returns a non-zero value on success, or a zero value
 * on failure.
 */
int Lexer_Lex(
    struct Lexer* lexer, const struct Colonini_String* lines, size_t count);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_LEXER_H_ */
