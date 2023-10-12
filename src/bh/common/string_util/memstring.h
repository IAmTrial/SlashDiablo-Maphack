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

#ifndef BH_COMMON_STRING_UTIL_MEMSTRING_H_
#define BH_COMMON_STRING_UTIL_MEMSTRING_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * Finds characters NOT in the search, similar to strcspn, but with
 * size specifiers similar to the mem-variant functions. Searches in
 * the reverse direction.
 */
size_t MemCRSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size);

/**
 * Finds characters NOT in the search, similar to strcspn, but with
 * size specifiers similar to the mem-variant functions. Searches in
 * the reverse direction.
 */
size_t WMemCRSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size);

/**
 * Finds characters NOT in the search, similar to strcspn, but with
 * size specifiers similar to the mem-variant functions.
 */
size_t MemCSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size);

/**
 * Finds characters NOT in the search, similar to strcspn, but with
 * size specifiers similar to the mem-variant functions.
 */
size_t WMemCSpn(
    const wchar_t* data,
    size_t data_size,
    const wchar_t* search,
    size_t search_size);

/**
 * Finds characters in the search, similar to strspn, but with size
 * specifiers similar to the mem-variant functions. Searches in
 * the reverse direction.
 */
size_t MemRSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size);

/**
 * Finds characters in the search, similar to strspn, but with size
 * specifiers similar to the mem-variant functions. Searches in
 * the reverse direction.
 */
size_t WMemRSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size);

/**
 * Finds characters in the search, similar to strspn, but with size
 * specifiers similar to the mem-variant functions.
 */
size_t MemSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size);

/**
 * Finds characters in the search, similar to strspn, but with size
 * specifiers similar to the mem-variant functions.
 */
size_t WMemSpn(
    const wchar_t* data,
    size_t data_size,
    const wchar_t* search,
    size_t search_size);

/**
 * Finds a substring in a source string, similar to strstr, but with
 * length specifiers similar to the mem-variant functions.
 */
void* MemStr(
    const void* data, size_t data_size, const void* sub, size_t sub_size);

/**
 * Finds a substring in a source string, similar to strstr, but with
 * length specifiers similar to the mem-variant functions.
 */
wchar_t* WMemStr(
    const wchar_t* data,
    size_t data_size,
    const wchar_t* sub,
    size_t sub_size);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_STRING_UTIL_MEMSTRING_H_ */
