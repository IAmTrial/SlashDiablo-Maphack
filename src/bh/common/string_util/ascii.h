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

#ifndef BH_COMMON_STRING_UTIL_ASCII_H_
#define BH_COMMON_STRING_UTIL_ASCII_H_

#include <stddef.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * Returns the lowercase character for a specified character. If the
 * character is not an uppercase character for 7-bit ASCII, then the
 * function returns the unmodified character.
 */
char Ascii_ToLowerChar(char ch);

/**
 * Returns the lowercase character for a specified character. If the
 * character is not an uppercase character for 7-bit ASCII, then the
 * function returns the unmodified character.
 */
wchar_t Ascii_ToLowerWChar(wchar_t ch);

/**
 * Returns a null-terminated copy of the specified string with all
 * uppercase 7-bit ASCII characters converted to lowercase.
 */
char* Ascii_ToLowerStr(char* dest, const char* src, size_t length);

/**
 * Returns a null-terminated copy of the specified string with all
 * uppercase 7-bit ASCII characters converted to lowercase.
 */
wchar_t* Ascii_ToLowerWStr(wchar_t* dest, const wchar_t* src, size_t length);

/**
 * Returns the uppercase character for a specified character. If the
 * character is not a lowercase character for 7-bit ASCII, then the
 * function returns the unmodified character.
 */
char Ascii_ToUpperChar(char ch);

/**
 * Returns the uppercase character for a specified character. If the
 * character is not a lowercase character for 7-bit ASCII, then the
 * function returns the unmodified character.
 */
wchar_t Ascii_ToUpperWChar(wchar_t ch);

/**
 * Returns a null-terminated copy of the specified string with all
 * lowercase 7-bit ASCII characters converted to uppercase.
 */
char* Ascii_ToUpperStr(char* dest, const char* src, size_t length);

/**
 * Returns a null-terminated copy of the specified string with all
 * lowercase 7-bit ASCII characters converted to uppercase.
 */
wchar_t* Ascii_ToUpperWStr(wchar_t* dest, const wchar_t* src, size_t length);

/**
 * Returns a null-terminated copy of the source string with all prefix
 * and suffix whitespace characters excluded from the destination
 * string.
 */
char* Ascii_TrimWhitespaceChars(char* dest, const char* src, size_t length);

/**
 * Returns a null-terminated copy of the source string with all prefix
 * and suffix whitespace characters excluded from the destination
 * string.
 */
wchar_t* Ascii_TrimWhitespaceWChars(
    wchar_t* dest, const wchar_t* src, size_t length);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_STRING_UTIL_ASCII_H_ */
