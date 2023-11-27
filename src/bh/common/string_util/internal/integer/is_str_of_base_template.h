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
#include "bh/common/string_util/internal/integer/get_base_from_prefix_str.h"
#include "bh/common/string_util/internal/integer/is_digit_char_of_base.h"
#include "bh/common/string_util/internal/integer/is_str_of_base.h"

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

#if !defined(T_STR_LITERAL_PREFIX)
#error Define T_STR_LITERAL_PREFIX to specify the templated string literal prefix.
#endif  /* !defined(T_STR_LITERAL_PREFIX) */

#define TEXT_LITERAL(lit) PREPROCESSOR_CONCAT(T_STR_LITERAL_PREFIX, lit)

int T_Integer_IsStrOfBase(T_CHAR)(const T_CHAR* str, size_t length, int base) {
  size_t i;
  size_t i_start;
  int guess_base;
  int* guess_base_result;
  int is_negative;

  assert(str != NULL);

  if (length <= 0) {
    return 0;
  }

  is_negative = (str[0] == TEXT_LITERAL('-'));
  i_start = (is_negative) ? 1 : 0;

  guess_base_result =
      T_Integer_GetBaseFromPrefixStr(T_CHAR)(&guess_base, str, length);
  if (guess_base_result != NULL) {
    switch (guess_base) {
      case 8:
      case 16: {
        if (guess_base != base) {
          return 0;
        }
        i_start += (guess_base == 8) ? 1 : 2;
        break;
      }

      case 10: {
        break;
      }

      default: {
        assert(0 && "This should never happen.");
        return 0;
      }
    }
  } else {
    i_start = 0;
  }

  if (i_start >= length) {
    return 0;
  }

  for (i = i_start; i < length; ++i) {
    if (!T_Integer_IsDigitCharOfBase(T_CHAR)(str[i], base)) {
      return 0;
    }
  }

  return 1;
}

#undef TEXT_LITERAL

#undef T_STR_LITERAL_PREFIX
#undef T_CHAR
