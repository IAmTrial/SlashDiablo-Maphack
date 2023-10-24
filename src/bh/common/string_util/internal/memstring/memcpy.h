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

#ifndef BH_COMMON_STRING_UTIL_INTERNAL_MEMSTRING_MEMCPY_H_
#define BH_COMMON_STRING_UTIL_INTERNAL_MEMSTRING_MEMCPY_H_

#include <stddef.h>
#include <wchar.h>

#include "bh/common/preprocessor/template/identifier.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define T_MemCpy(T_CHAR) TEMPLATE_IDENTIFIER_1(T_MemCpy, T_CHAR)

void* T_MemCpy(void)(void* dest, const void* src, size_t count);
char* T_MemCpy(char)(char* dest, const char* src, size_t count);
wchar_t* T_MemCpy(wchar_t)(wchar_t* dest, const wchar_t* src, size_t count);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_STRING_UTIL_INTERNAL_MEMSTRING_MEMCPY_H_ */
