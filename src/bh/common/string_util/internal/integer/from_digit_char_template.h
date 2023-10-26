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
#include "bh/common/string_util/internal/integer/from_digit_char.h"

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

#if !defined(T_STR_LITERAL_PREFIX)
#error Define T_STR_LITERAL_PREFIX to specify the templated string literal prefix.
#endif  /* !defined(T_STR_LITERAL_PREFIX) */

#define TEXT_LITERAL(lit) PREPROCESSOR_CONCAT(T_STR_LITERAL_PREFIX, lit)

int* T_Integer_FromDigitChar(T_CHAR)(int* value, T_CHAR ch, int base) {
  T_CHAR lowercase;
  int temp_value;

  assert(value != NULL);

  /* Only the values in interval [1, 36] are valid for base. */
  if (base < 1 || base > 36) {
    return NULL;
  }

  lowercase = T_Ascii_ToLowerChar(T_CHAR)(ch);
  if (ch >= TEXT_LITERAL('0') && ch <= TEXT_LITERAL('9')) {
    temp_value = ch - TEXT_LITERAL('0');
  } else if (lowercase >= TEXT_LITERAL('a')
      && lowercase <= TEXT_LITERAL('z')) {
    temp_value = lowercase - TEXT_LITERAL('a') + 10;
  } else {
    return NULL;
  }

  if (temp_value >= base) {
    return NULL;
  }

  *value = temp_value;
  return value;
}

#undef TEXT_LITERAL

#undef T_STR_LITERAL_PREFIX
#undef T_CHAR
