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
#include "bh/common/string_util/internal/integer/from_str_of_base_to_int.h"
#include "bh/common/string_util/internal/integer/get_base_from_prefix_str.h"

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

#if !defined(T_INT)
#error Define T_INT to specify the templated integer type.
#endif  /* !defined(T_INT) */

T_INT* T_Integer_FromStrToInt(T_CHAR, T_INT)(
    T_INT* value, const T_CHAR* str, size_t length) {
  int base;
  int* base_get_result;

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

  return T_Integer_FromStrOfBaseToInt(T_CHAR, T_INT)(value, str, length, base);
}

#undef T_INT
#undef T_CHAR
