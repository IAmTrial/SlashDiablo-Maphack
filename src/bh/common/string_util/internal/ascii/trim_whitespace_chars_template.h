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
#include "bh/common/string_util/internal/ascii/trim_whitespace_chars.h"
#include "bh/common/string_util/internal/memstring/memcpy.h"
#include "bh/common/string_util/internal/memstring/memcrspn.h"
#include "bh/common/string_util/internal/memstring/memcspn.h"

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

#if !defined(T_STR_LITERAL_PREFIX)
#error Define T_STR_LITERAL_PREFIX to specify the templated string literal prefix.
#endif  /* !defined(T_STR_LITERAL_PREFIX) */

#define TEXT_LITERAL(lit) PREPROCESSOR_CONCAT(T_STR_LITERAL_PREFIX, lit)

T_CHAR* T_Ascii_TrimWhitespaceChars(T_CHAR)(
    T_CHAR* dest, const T_CHAR* src, size_t length) {
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

  i_begin =
      T_MemCSpn(T_CHAR)(src, length, kWhitespaceChars, kWhitespaceCharsCount);
  dest_length =
      T_MemCRSpn(T_CHAR)(
          &src[i_begin],
          length - i_begin,
          kWhitespaceChars,
          kWhitespaceCharsCount);

  T_MemCpy(T_CHAR)(dest, &src[i_begin], dest_length + 1);

  return dest;
}

#undef TEXT_LITERAL

#undef T_STR_LITERAL_PREFIX
#undef T_CHAR
