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

#ifndef BH_COMMON_STRING_UTIL_INTERNAL_INTEGER_FROM_STR_TO_INT_H_
#define BH_COMMON_STRING_UTIL_INTERNAL_INTEGER_FROM_STR_TO_INT_H_

#include <stddef.h>
#include <wchar.h>

#include "bh/common/preprocessor/template/identifier.h"
#include "bh/common/preprocessor/template/typedef.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define T_Integer_FromStrToInt(T_CHAR, T_INT) \
    TEMPLATE_IDENTIFIER_2(T_Integer_FromStrToInt, T_CHAR, T_INT)

signed_char* T_Integer_FromStrToInt(char, signed_char)(
    signed_char* value, const char* str, size_t length);

signed_char* T_Integer_FromStrToInt(wchar_t, signed_char)(
    signed_char* value, const wchar_t* str, size_t length);

short* T_Integer_FromStrToInt(char, short)(
    short* value, const char* str, size_t length);

short* T_Integer_FromStrToInt(wchar_t, short)(
    short* value, const wchar_t* str, size_t length);

int* T_Integer_FromStrToInt(char, int)(
    int* value, const char* str, size_t length);

int* T_Integer_FromStrToInt(wchar_t, int)(
    int* value, const wchar_t* str, size_t length);

long* T_Integer_FromStrToInt(char, long)(
    long* value, const char* str, size_t length);

long* T_Integer_FromStrToInt(wchar_t, long)(
    long* value, const wchar_t* str, size_t length);

unsigned_char* T_Integer_FromStrToInt(char, unsigned_char)(
    unsigned_char* value, const char* str, size_t length);

unsigned_char* T_Integer_FromStrToInt(wchar_t, unsigned_char)(
    unsigned_char* value, const wchar_t* str, size_t length);

unsigned_short* T_Integer_FromStrToInt(char, unsigned_short)(
    unsigned_short* value, const char* str, size_t length);

unsigned_short* T_Integer_FromStrToInt(wchar_t, unsigned_short)(
    unsigned_short* value, const wchar_t* str, size_t length);

unsigned_int* T_Integer_FromStrToInt(char, unsigned_int)(
    unsigned_int* value, const char* str, size_t length);

unsigned_int* T_Integer_FromStrToInt(wchar_t, unsigned_int)(
    unsigned_int* value, const wchar_t* str, size_t length);

unsigned_long* T_Integer_FromStrToInt(char, unsigned_long)(
    unsigned_long* value, const char* str, size_t length);

unsigned_long* T_Integer_FromStrToInt(wchar_t, unsigned_long)(
    unsigned_long* value, const wchar_t* str, size_t length);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_STRING_UTIL_INTERNAL_INTEGER_FROM_STR_TO_INT_H_ */
