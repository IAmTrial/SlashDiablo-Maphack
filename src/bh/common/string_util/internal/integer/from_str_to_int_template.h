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
#include "bh/common/string_util/internal/integer/from_digit_char_of_base.h"
#include "bh/common/string_util/internal/integer/from_str_to_int.h"
#include "bh/common/string_util/internal/integer/get_base_from_prefix_str.h"

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

#if !defined(T_STR_LITERAL_PREFIX)
#error Define T_STR_LITERAL_PREFIX to specify the templated string literal prefix.
#endif  /* !defined(T_STR_LITERAL_PREFIX) */

#if !defined(T_INT)
#error Define T_INT to specify the templated integer type.
#endif  /* !defined(T_INT) */

#if !defined(T_INT_MIN)
#error Define T_INT_MIN to specify the templated minimum value of the integer type.
#endif  /* !defined(T_INT_MIN) */

#if !defined(T_INT_MAX)
#error Define T_INT_MAX to specify the templated maximum value of the integer type.
#endif  /* !defined(T_INT_MAX) */

#define TEXT_LITERAL(lit) PREPROCESSOR_CONCAT(T_STR_LITERAL_PREFIX, lit)

T_INT* T_Integer_FromStrToInt(T_CHAR, T_INT)(
    T_INT* value, const T_CHAR* str, size_t length) {
  size_t i;
  int base;
  int* base_get_result;
  int is_negative;
  size_t i_start;
  T_INT temp_value;

  assert(value != NULL);
  assert(str != NULL);

  if (length <= 0) {
    return NULL;
  }

  /*
   * Check if the digit is valid for the detected prefix's base. If it
   * is valid, get the value.
   */
  base_get_result = T_Integer_GetBaseFromPrefixStr(T_CHAR)(&base, str, length);
  if (base_get_result == NULL) {
    return NULL;
  }

  /* Do not allow unsigned numbers to have negative numbers. */
  is_negative = (str[0] == TEXT_LITERAL('-'));
  if (T_INT_MIN >= (T_INT)0 && is_negative) {
    return NULL;
  }

  i_start = is_negative ? 1 : 0;
  switch (base) {
    case 8: {
      i_start += 1;
      break;
    }

    case 10: {
      break;
    }

    case 16: {
      i_start += 2;
      break;
    }

    default: {
      assert(0 && "This should never happen.");
      return NULL;
    }
  }

  if (i_start >= length) {
    return NULL;
  }

  /* Convert the string to the integer type. */
  temp_value = 0;
  for (i = i_start; i < length; ++i) {
    int digit;
    int* digit_convert_result;

    /* Check if the string has too many digits. */
    if ((is_negative && temp_value < (T_INT_MIN / base))
        || (!is_negative && temp_value > (T_INT_MAX / base))) {
      return NULL;
    }
    temp_value *= base;

    /*
     * Check if the digit is valid for the detected prefix's base. If
     * it is valid, get the value.
     */
    digit_convert_result =
        T_Integer_FromDigitCharOfBase(T_CHAR)(&digit, str[i], base);
    if (digit_convert_result == NULL) {
      return NULL;
    }

    /*
     * Check if add/sub of this digit to the value would go over/under
     * the limit.
     */
    if ((is_negative && temp_value < (T_INT_MIN + digit))
        || (!is_negative && temp_value > (T_INT_MAX - digit))) {
      return NULL;
    }

    temp_value += (is_negative ? -1 : 1) * digit;
  }

  *value = temp_value;
  return value;
}

#undef TEXT_LITERAL

#undef T_INT_MAX
#undef T_INT_MIN
#undef T_INT
#undef T_STR_LITERAL_PREFIX
#undef T_CHAR
