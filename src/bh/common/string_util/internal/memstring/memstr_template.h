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

#include "bh/common/string_util/internal/memstring/memchr.h"
#include "bh/common/string_util/internal/memstring/memcmp.h"
#include "bh/common/string_util/internal/memstring/memstr.h"

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

T_CHAR* T_MemStr(T_CHAR)(
    const T_CHAR* data,
    size_t data_size,
    const T_CHAR* sub,
    size_t sub_size) {
  const T_CHAR* str;
  const T_CHAR* substr;
  const T_CHAR* first_ch;

  if (data_size <= 0) {
    return NULL;
  }

  if (sub_size <= 0) {
    return NULL;
  }

  str = data;
  substr = sub;

  first_ch = str;
  do {
    int compare_result;
    size_t str_remaining_size;

    str_remaining_size = &str[data_size] - first_ch;
    if (str_remaining_size < sub_size) {
      return NULL;
    }

    compare_result = T_MemCmp(T_CHAR)(first_ch, sub, sub_size);
    if (compare_result == 0) {
      return (T_CHAR*)first_ch;
    }

    first_ch = T_MemChr(T_CHAR)(&first_ch[1], substr[0], str_remaining_size);
  } while (first_ch != NULL);

  return NULL;
}

#undef T_CHAR
