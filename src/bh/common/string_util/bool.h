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

#ifndef BH_COMMON_STRING_UTIL_BOOL_H_
#define BH_COMMON_STRING_UTIL_BOOL_H_

#include <stddef.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * Sets destination value to a non-zero value if the source string is
 * equal to "on" via case-insensitive comparison. Sets value to zero
 * if the same comparison applies to "off". Returns a non-NULL
 * pointer in those two cases. Otherwise, does not set the destination
 * value and returns NULL.
 */
int* Bool_FromOnOffStr(int* dest, const char* src, size_t length);

/**
 * Sets destination value to a non-zero value if the source string is
 * equal to "on" via case-insensitive comparison. Sets value to zero
 * if the same comparison applies to "off". Returns a non-NULL
 * pointer in those two cases. Otherwise, does not set the destination
 * value and returns NULL.
 */
int* Bool_FromOnOffWStr(int* dest, const wchar_t* src, size_t length);

/**
 * Sets destination value to a non-zero value if the source string is
 * equal to "true" via case-insensitive comparison. Sets value to zero
 * if the same comparison applies to "false". Returns a non-NULL
 * pointer in those two cases. Otherwise, does not set the destination
 * value and returns NULL.
 */
int* Bool_FromTrueFalseStr(int* dest, const char* src, size_t length);

/**
 * Sets destination value to a non-zero value if the source string is
 * equal to "true" via case-insensitive comparison. Sets value to zero
 * if the same comparison applies to "false". Returns a non-NULL
 * pointer in those two cases. Otherwise, does not set the destination
 * value and returns NULL.
 */
int* Bool_FromTrueFalseWStr(int* dest, const wchar_t* src, size_t length);

/**
 * Returns "on" if the value is non-zero, or "off" if the value is
 * zero. The string is null-terminated.
 */
const char* Bool_ToOnOffStr(int value, size_t* length);

/**
 * Returns "on" if the value is non-zero, or "off" if the value is
 * zero. The string is null-terminated.
 */
const wchar_t* Bool_ToOnOffWStr(int value, size_t* length);

/**
 * Returns "true" if the value is non-zero, or "false" if the value is
 * zero. The string is null-terminated.
 */
const char* Bool_ToTrueFalseStr(int value, size_t* length);

/**
 * Returns "true" if the value is non-zero, or "false" if the value is
 * zero. The string is null-terminated.
 */
const wchar_t* Bool_ToTrueFalseWStr(int value, size_t* length);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_STRING_UTIL_BOOL_H_ */
