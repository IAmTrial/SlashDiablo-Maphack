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

/**
 * BH
 * Copyright (C) 2011  McGod
 *
 * All rights reserved.
 */

#include <stddef.h>

#include "bh/common/preprocessor/concat.h"
#include "bh/common/string_util/internal/ascii/to_lower_char.h"
#include "bh/common/string_util/internal/ascii/to_lower_str.h"

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

#if !defined(T_STR_LITERAL_PREFIX)
#error Define T_STR_LITERAL_PREFIX to specify the templated string literal prefix.
#endif  /* !defined(T_STR_LITERAL_PREFIX) */

#define TEXT_LITERAL(lit) PREPROCESSOR_CONCAT(T_STR_LITERAL_PREFIX, lit)

T_CHAR* T_Ascii_ToLowerStr(T_CHAR)(
    T_CHAR* dest, const T_CHAR* src, size_t length) {
  size_t i;

  for (i = 0; i < length; ++i) {
    dest[i] = T_Ascii_ToLowerChar(T_CHAR)(src[i]);
  }
  dest[i] = TEXT_LITERAL('\0');

  return dest;
}

#undef TEXT_LITERAL

#undef T_STR_LITERAL_PREFIX
#undef T_CHAR
