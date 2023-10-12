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

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

#if !defined(T_STR_LITERAL_PREFIX)
#error Define T_STR_LITERAL_PREFIX to specify the templated string literal prefix.
#endif  /* !defined(T_STR_LITERAL_PREFIX) */

#if !defined(T_MEMCRSPN_FUNC_NAME)
#error Define T_MEMCRSPN_FUNC_NAME to specify the memcrspn function name.
#endif  /* !defined(T_MEMCRSPN_FUNC_NAME) */

#if !defined(T_MEMCPY_FUNC_NAME)
#error Define T_MEMCPY_FUNC_NAME to specify the memcpy function name.
#endif  /* !defined(T_MEMCPY_FUNC_NAME) */

#if !defined(T_MEMCSPN_FUNC_NAME)
#error Define T_MEMCSPN_FUNC_NAME to specify the memcspn function name.
#endif  /* !defined(T_MEMCSPN_FUNC_NAME) */

#if !defined(T_FUNC_NAME)
#error Define T_FUNC_NAME to specify the function name.
#endif  /* !defined(T_FUNC_NAME) */

#define CONCAT_IMPL(a, b) a ## b
#define CONCAT(a, b) CONCAT_IMPL(a, b)
#define TEXT_LITERAL(lit) CONCAT(T_STR_LITERAL_PREFIX, lit)

T_CHAR* T_FUNC_NAME(T_CHAR* dest, const T_CHAR* src, size_t length) {
  static const T_CHAR kWhitespaceChars[] = {
    TEXT_LITERAL(' '),
    TEXT_LITERAL('\f'),
    TEXT_LITERAL('\n'),
    TEXT_LITERAL('\r'),
    TEXT_LITERAL('\t'),
    TEXT_LITERAL('\v'),
    TEXT_LITERAL('\0')
  };
  enum {
    kWhitespaceCharsCount =
        sizeof(kWhitespaceChars) / sizeof(kWhitespaceChars[0])
  };

  size_t i_begin;
  size_t dest_length;

  i_begin = T_MEMCSPN_FUNC_NAME(src, length, kWhitespaceChars, kWhitespaceCharsCount);
  dest_length =
      T_MEMCRSPN_FUNC_NAME(
          &src[i_begin],
          length - i_begin,
          kWhitespaceChars,
          kWhitespaceCharsCount);

  T_MEMCPY_FUNC_NAME(dest, &src[i_begin], dest_length + 1);

  return dest;
}

#undef TEXT_LITERAL
#undef CONCAT
#undef CONCAT_IMPL
#undef T_FUNC_NAME
#undef T_MEMCSPN_FUNC_NAME
#undef T_MEMCPY_FUNC_NAME
#undef T_MEMCRSPN_FUNC_NAME
#undef T_STR_LITERAL_PREFIX
#undef T_CHAR
