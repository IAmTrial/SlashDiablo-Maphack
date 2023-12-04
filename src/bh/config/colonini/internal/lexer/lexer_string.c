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

#include "bh/config/colonini/internal/lexer/lexer_string.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "bh/config/colonini/type/string.h"

struct LexerString* LexerString_InitStrComponent(
    struct LexerString* lstr,
    const char* src,
    size_t length,
    size_t line_index) {
  struct Colonini_String* str_init_result;
  struct Colonini_String* str_concat_result;

  str_init_result = Colonini_String_Init(&lstr->str, length);
  if (str_init_result == NULL) {
    goto error;
  }

  str_concat_result = Colonini_String_Concat(&lstr->str, src, length);
  if (str_concat_result == NULL) {
    goto error_deinit_str;
  }

  lstr->line_index = line_index;

  return lstr;

error_deinit_str:
  Colonini_String_Deinit(&lstr->str);

error:
  return NULL;
}

void LexerString_Deinit(struct LexerString* lstr) {
  lstr->next_token = NULL;
  lstr->previous_token = NULL;
  Colonini_String_Deinit(&lstr->str);
}

struct LexerString* LexerString_CeilToken(const struct LexerString* lstr) {
  if (isspace(lstr->str.str[0])) {
    return lstr->next_token;
  }

  return (struct LexerString*)lstr;
}

struct LexerString* LexerString_FloorToken(const struct LexerString* lstr) {
  if (isspace(lstr->str.str[0])) {
    return lstr->previous_token;
  }

  return (struct LexerString*)lstr;
}
