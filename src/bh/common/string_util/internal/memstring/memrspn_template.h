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

#include "bh/common/string_util/internal/memstring/memrspn.h"

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

size_t T_MemRSpn(T_CHAR)(
    const T_CHAR* data,
    size_t data_size,
    const T_CHAR* search,
    size_t search_size) {
  const T_CHAR* str;
  size_t i_str;
  const T_CHAR* chars;
  size_t i_chars;

  if (data_size <= 0) {
    return data_size;
  }

  if (search_size <= 0) {
    return data_size;
  }

  str = data;
  chars = search;

  /* Scan str for a character that is in chars. */
  for (i_str = data_size; i_str-- > 0; ) {
    for (i_chars = 0; i_chars < search_size; ++i_chars) {
      if (str[i_str] == chars[i_chars]) {
        return i_str;
      }
    }
  }

  return data_size;
}

#undef T_CHAR
