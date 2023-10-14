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

#include "bh/config/colonini/internal/lexer.h"

#include <stddef.h>
#include <stdlib.h>

#include "bh/config/colonini/internal/lexer/lexer_line.h"
#include "bh/config/colonini/type/string.h"

/**
 * External
 */

struct Lexer* Lexer_Init(struct Lexer* lexer, size_t line_count) {
  lexer->lines = malloc(line_count * sizeof(lexer->lines[0]));
  if (lexer->lines == NULL) {
    goto error;
  }
  lexer->count = 0;

  return lexer;

error:
  return NULL;
}

void Lexer_Deinit(struct Lexer* lexer) {
  for (; lexer->count > 0; --lexer->count) {
    LexerLine_Deinit(&lexer->lines[lexer->count - 1]);
  }
  free(lexer->lines);
  lexer->lines = NULL;
}

int Lexer_Lex(
    struct Lexer* lexer, const struct Colonini_String* lines, size_t count) {
  size_t i;

  for (i = 0; i < count; ++i) {
    struct LexerLine* lex_result;

    lex_result =
        LexerLine_LexLine(&lexer->lines[i], i + 1, &lines[i]);
    if (lex_result == NULL) {
      return 0;
    }
  }

  return 1;
}
