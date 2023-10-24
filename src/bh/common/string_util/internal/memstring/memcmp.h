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

#ifndef BH_COMMON_STRING_UTIL_INTERNAL_MEMSTRING_MEMCMP_H_
#define BH_COMMON_STRING_UTIL_INTERNAL_MEMSTRING_MEMCMP_H_

#include <stddef.h>
#include <wchar.h>

#include "bh/common/preprocessor/template/identifier.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define T_MemCmp(T_INPUT) TEMPLATE_IDENTIFIER_1(T_MemCmp, T_INPUT)

int T_MemCmp(void)(const void* left, const void* right, size_t count);
int T_MemCmp(char)(const char* left, const char* right, size_t count);
int T_MemCmp(wchar_t)(const wchar_t* left, const wchar_t* right, size_t count);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_STRING_UTIL_INTERNAL_MEMSTRING_MEMCMP_H_ */
