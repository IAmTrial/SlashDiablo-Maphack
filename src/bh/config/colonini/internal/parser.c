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

#include "bh/config/colonini/internal/parser.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "bh/config/colonini/internal/lexer.h"
#include "bh/config/colonini/internal/parser/parser_line.h"

/**
 * External
 */

struct Parser* Parser_Init(struct Parser* parser, size_t line_count) {
  parser->lines = malloc(line_count * sizeof(parser->lines[0]));
  if (parser->lines == NULL) {
    goto error;
  }
  parser->line_count = 0;

  return parser;

error:
  return NULL;
}

void Parser_Deinit(struct Parser* parser) {
  for (; parser->line_count > 0; --parser->line_count) {
    ParserLine_Deinit(&parser->lines[parser->line_count - 1]);
  }
  free(parser->lines);
  parser->lines = NULL;
}

int Parser_Parse(
    struct Parser* parser, struct Lexer* lexer, size_t* error_column) {
  for (parser->line_count = 0;
      parser->line_count < lexer->count;
      ++parser->line_count) {
    struct ParserLine* parse_result;
      
    parse_result =
        ParserLine_ParseLine(
            &parser->lines[parser->line_count],
            &lexer->lines[parser->line_count],
            error_column);
    if (parse_result == NULL) {
      return 0;
    }
  }

  return 1;
}
