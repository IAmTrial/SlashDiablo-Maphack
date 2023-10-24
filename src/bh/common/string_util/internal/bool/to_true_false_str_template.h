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

#if !defined(T_TRUE_STR_LITERAL)
#error Define T_TRUE_STR_LITERAL to specify the string literal for the true value.
#endif  /* !defined(T_TRUE_STR_LITERAL) */

#if !defined(T_FALSE_STR_LITERAL)
#error Define T_FALSE_STR_LITERAL to specify the string literal for the false value.
#endif  /* !defined(T_FALSE_STR_LITERAL) */

#if !defined(T_FUNC_NAME)
#error Define T_FUNC_NAME to specify the function name.
#endif  /* !defined(T_FUNC_NAME) */

const T_CHAR* T_FUNC_NAME(T_CHAR)(int value, size_t* length) {
  if (value) {
    static const T_CHAR kStr[] = T_TRUE_STR_LITERAL;

    if (length != NULL) {
      *length = sizeof(kStr) / sizeof(kStr[0]) - 1;
    }

    return kStr;
  } else {
    static const T_CHAR kStr[] = T_FALSE_STR_LITERAL;

    if (length != NULL) {
      *length = sizeof(kStr) / sizeof(kStr[0]) - 1;
    }

    return kStr;
  }
}

#undef T_FUNC_NAME

#undef T_FALSE_STR_LITERAL
#undef T_TRUE_STR_LITERAL

#undef T_CHAR
