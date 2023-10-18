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

#include <stddef.h>
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

/**
 * Determines the integer-base encoding that matches the contents of
 * the string's prefix. If the integer-base encoding is detected, then
 * the function returns a non-NULL value. Otherwise, returns NULL on
 * failure.
 *
 * Only supports detection of octal, decimal, and hexadecimal. Does not
 * check the actual contents of the string.
 */
int* Integer_GetBaseFromPrefixStr(int* base, const char* str, size_t length);

/**
 * Determines the integer-base encoding that matches the contents of
 * the string's prefix. If the integer-base encoding is detected, then
 * the function returns a non-NULL value. Otherwise, returns NULL on
 * failure.
 *
 * Only supports detection of octal, decimal, and hexadecimal. Does not
 * check the actual contents of the string.
 */
int* Integer_GetBaseFromPrefixWStr(
    int* base, const wchar_t* str, size_t length);

/**
 * Returns a non-zero value if the specified character is representable
 * as a number with the specified integer-base encoding. Otherwise,
 * returns zero.
 *
 * Only the values in the interval [1, 36] are valid for base. The
 * function will always return zero if an invalid base is specified.
 */
int Integer_IsDigitCharOfBase(char ch, int base);


/**
 * Returns a non-zero value if the specified character is representable
 * as a number with the specified integer-base encoding. Otherwise,
 * returns zero.
 *
 * Only the values in the interval [1, 36] are valid for base. The
 * function will always return zero if an invalid base is specified.
 */
int Integer_IsDigitWCharOfBase(wchar_t ch, int base);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_STRING_UTIL_INTEGER_H_ */
