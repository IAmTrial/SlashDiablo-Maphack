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

#ifndef BH_COMMON_STRING_UTIL_INTEGER_H_
#define BH_COMMON_STRING_UTIL_INTEGER_H_

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * Converts a character digit into an integer value and sets the
 * output to that value. Returns a non-NULL pointer if the digit
 * was converted, or NULL if the specified character is not a digit.
 */
int* Integer_FromDigitChar(int* value, char ch, int base);

/**
 * Converts a character digit into an integer value and sets the
 * output to that value. Returns a non-NULL pointer if the digit
 * was converted, or NULL if the specified character is not a digit.
 */
int* Integer_FromDigitWChar(int* value, wchar_t ch, int base);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_STRING_UTIL_INTEGER_H_ */
