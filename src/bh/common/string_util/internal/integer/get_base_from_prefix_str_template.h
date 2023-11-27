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

#include <assert.h>
#include <stddef.h>

#include "bh/common/preprocessor/concat.h"
#include "bh/common/string_util/internal/ascii/to_lower_char.h"
#include "bh/common/string_util/internal/integer/get_base_from_prefix_str.h"

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

#if !defined(T_STR_LITERAL_PREFIX)
#error Define T_STR_LITERAL_PREFIX to specify the templated string literal prefix.
#endif  /* !defined(T_STR_LITERAL_PREFIX) */

#define TEXT_LITERAL(lit) PREPROCESSOR_CONCAT(T_STR_LITERAL_PREFIX, lit)

int* T_Integer_GetBaseFromPrefixStr(T_CHAR)(
    int* base, const T_CHAR* str, size_t length) {
  int is_negative;
  size_t i_start;

  assert(base != NULL);
  assert(str != NULL);

  if (length == 0) {
    return NULL;
  }

  /* Track the negative sign. */
  is_negative = (str[0] == TEXT_LITERAL('-'));

  i_start = is_negative ? 1 : 0;
  if (i_start >= length) {
    return NULL;
  }

  if (str[i_start] == TEXT_LITERAL('0')) {
    if (length == i_start + 1) {
      /* The string is "0" or "-0", so the prefix is decimal. */
      *base = 10;
      return base;
    }
    assert(length > i_start + 1);

    if (T_Ascii_ToLowerChar(T_CHAR)(str[i_start + 1]) == TEXT_LITERAL('x')) {
      /* The string starts with "0x" or "0X", so the prefix is hexadecimal. */
      *base = 16;
      return base;
    }

    /* The string starts with "0", so the prefix is octal. */
    *base = 8;
    return base;
  }

  if (str[i_start] >= TEXT_LITERAL('1') && str[i_start] <= TEXT_LITERAL('9')) {
    /* The prefix is decimal. */
    *base = 10;
    return base;
  }

  return NULL;
}

#undef TEXT_LITERAL

#undef T_STR_LITERAL_PREFIX
#undef T_CHAR
