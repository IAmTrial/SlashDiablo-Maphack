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

#ifndef BH_CONFIG_COLONINI_INTERNAL_PARSER_H_
#define BH_CONFIG_COLONINI_INTERNAL_PARSER_H_

#include <stddef.h>

#include "bh/config/colonini/internal/lexer.h"
#include "bh/config/colonini/internal/parser/parser_line.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct Parser {
  size_t line_count;
  struct ParserLine* lines;
};

/**
 * Initializes a Parser with space allocated for the specified number
 * of ParserLine.
 */
struct Parser* Parser_Init(struct Parser* parser, size_t line_count);

/**
 * Deinitializes a Parser, freeing up resources that were allocated.
 */
void Parser_Deinit(struct Parser* parser);

/**
 * Parses the state of Colonini lines from the Lexer. Returns a
 * non-zero value on success, or returns zero and sets error column on
 * failure.
 */
int Parser_Parse(
    struct Parser* parser, struct Lexer* lexer, size_t* error_column);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_PARSER_H_ */
