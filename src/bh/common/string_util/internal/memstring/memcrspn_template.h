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

#include <stddef.h>

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

#if !defined(T_INPUT)
#error Define T_INPUT to specify the templated parameter type.
#endif  /* !defined(T_INPUT) */

#if !defined(T_FUNC_NAME)
#error Define T_FUNC_NAME to specify the function name.
#endif  /* !defined(T_FUNC_NAME) */

size_t T_FUNC_NAME(
    const T_INPUT* data,
    size_t data_size,
    const T_INPUT* search,
    size_t search_size) {
  const T_CHAR* str;
  size_t i_str;
  const T_CHAR* chars;

  if (data_size <= 0) {
    return 0;
  }

  if (search_size <= 0) {
    return data_size - 1;
  }

  str = data;
  chars = search;

  /* Scan str for a character that is not in chars. */
  for (i_str = data_size; i_str-- > 0; ) {
    size_t i_chars;

    for (i_chars = 0; i_chars < search_size; ++i_chars) {
      if (str[i_str] == chars[i_chars]) {
        break;
      }
    }

    if (i_chars >= search_size) {
      return i_str;
    }
  }

  return data_size;
}

#undef T_FUNC_NAME
#undef T_INPUT
#undef T_CHAR
