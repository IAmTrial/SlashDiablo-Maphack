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

#include "bh/common/string_util/memstring.h"

#include <stddef.h>
#include <string.h>

size_t MemCSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size) {
  const char* str;
  size_t i_str;
  const char* chars;

  if (data_size <= 0) {
    return 0;
  }

  if (search_size <= 0) {
    return 0;
  }

  str = data;
  chars = search;

  /* Scan str for a character that is not in chars. */
  for (i_str = 0; i_str < data_size; ++i_str) {
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

void* MemStr(
    const void* data, size_t data_size, const void* sub, size_t sub_size) {
  const char* str;
  const char* substr;
  const char* first_ch;

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

    compare_result = memcmp(first_ch, sub, sub_size);
    if (compare_result == 0) {
      return (char*)first_ch;
    }

    first_ch =
        memchr(&first_ch[1], substr[0], str_remaining_size);
  } while (first_ch != NULL);

  return NULL;
}

size_t MemSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size) {
  const char* str;
  size_t i_str;
  const char* chars;
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
  for (i_str = 0; i_str < data_size; ++i_str) {
    for (i_chars = 0; i_chars < search_size; ++i_chars) {
      if (str[i_str] == chars[i_chars]) {
        return i_str;
      }
    }
  }

  return data_size;
}
