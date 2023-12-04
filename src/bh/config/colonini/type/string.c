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

#include "bh/config/colonini/type/string.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * External
 */

struct Colonini_String* Colonini_String_Init(
    struct Colonini_String* str, size_t count) {
  str->str = malloc((count + 1) * sizeof(str->str[0]));
  if (str->str == NULL) {
    goto error;
  }
  str->str[0] = '\0';
  str->length = 0;

  return str;

error:
  return NULL;
}

void Colonini_String_Deinit(struct Colonini_String* str) {
  str->length = 0;
  free(str->str);
  str->str = NULL;
}

struct Colonini_String* Colonini_String_Concat(
    struct Colonini_String* dest, const char* src, size_t src_length) {
  memcpy(&dest->str[dest->length], src, src_length);
  dest->str[dest->length + src_length] = '\0';
  dest->length += src_length;

  return dest;
}
