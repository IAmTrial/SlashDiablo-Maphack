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

#include "bh/common/string_util/internal/ascii/to_lower_str.h"
#include "bh/common/string_util/internal/bool/from_true_false_str.h"
#include "bh/common/string_util/internal/memstring/memcmp.h"

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

#if !defined(T_TRUE_STR_LITERAL)
#error Define T_TRUE_STR_LITERAL to specify the lowercase string literal for true value.
#endif  /* !defined(T_TRUE_STR_LITERAL) */

#if !defined(T_FALSE_STR_LITERAL)
#error Define T_FALSE_STR_LITERAL to specify the lowercase string literal for true value.
#endif  /* !defined(T_FALSE_STR_LITERAL) */

#if !defined(T_FUNC_NAME)
#error Define T_FUNC_NAME to specify the function name.
#endif  /* !defined(T_FUNC_NAME) */

int* T_FUNC_NAME(T_CHAR)(int* dest, const T_CHAR* src, size_t length) {
  static const T_CHAR kTrueStr[] = T_TRUE_STR_LITERAL;
  static const T_CHAR kFalseStr[] = T_FALSE_STR_LITERAL;
  enum {
    kTrueStrLength = sizeof(kTrueStr) / sizeof(kTrueStr[0]) - 1,
    kFalseStrLength = sizeof(kFalseStr) / sizeof(kFalseStr[0]) - 1,
    kLongerStrLength =
        (kTrueStrLength < kFalseStrLength) ? kFalseStrLength : kTrueStrLength
  };
  T_CHAR lower_str[kLongerStrLength + 1];
  const T_CHAR* compare_src;

#if !defined(NDEBUG)
  T_Ascii_ToLowerStr(T_CHAR)(lower_str, kTrueStr, kTrueStrLength);
  assert(T_MemCmp(T_CHAR)(lower_str, kTrueStr, kTrueStrLength) == 0);
  T_Ascii_ToLowerStr(T_CHAR)(lower_str, kFalseStr, kFalseStrLength);
  assert(T_MemCmp(T_CHAR)(lower_str, kFalseStr, kFalseStrLength) == 0);
#endif  /* !defined(NDEBUG) */

  if (src == NULL) {
    return NULL;
  }

  if (length != kTrueStrLength && length != kFalseStrLength) {
    return NULL;
  }

  T_Ascii_ToLowerStr(T_CHAR)(lower_str, src, length);

  if (T_MemCmp(T_CHAR)(lower_str, kTrueStr, kTrueStrLength) == 0) {
    *dest = 1;
    return dest;
  } else if (T_MemCmp(T_CHAR)(lower_str, kFalseStr, kFalseStrLength) == 0) {
    *dest = 0;
    return dest;
  } else {
    return NULL;
  }
}

#undef T_FUNC_NAME

#undef T_FALSE_STR_LITERAL
#undef T_TRUE_STR_LITERAL

#undef T_CHAR
