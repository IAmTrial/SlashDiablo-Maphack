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

#include "bh/common/string_util/memstring.h"

#include <stddef.h>
#include <wchar.h>

#include "bh/common/string_util/internal/memstring/memcrspn.h"
#include "bh/common/string_util/internal/memstring/memcspn.h"
#include "bh/common/string_util/internal/memstring/memrspn.h"
#include "bh/common/string_util/internal/memstring/memspn.h"
#include "bh/common/string_util/internal/memstring/memstr.h"

/**
 * External
 */

size_t MemCRSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size) {
  return T_MemCRSpn(void)(data, data_size, search, search_size);
}

size_t WMemCRSpn(
    const wchar_t* data,
    size_t data_size,
    const wchar_t* search,
    size_t search_size) {
  return T_MemCRSpn(wchar_t)(data, data_size, search, search_size);
}

size_t MemCSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size) {
  return T_MemCSpn(void)(data, data_size, search, search_size);
}

size_t WMemCSpn(
    const wchar_t* data,
    size_t data_size,
    const wchar_t* search,
    size_t search_size) {
  return T_MemCSpn(wchar_t)(data, data_size, search, search_size);
}

size_t MemRSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size) {
  return T_MemRSpn(void)(data, data_size, search, search_size);
}

size_t WMemRSpn(
    const wchar_t* data,
    size_t data_size,
    const wchar_t* search,
    size_t search_size) {
  return T_MemRSpn(wchar_t)(data, data_size, search, search_size);
}

size_t MemSpn(
    const void* data,
    size_t data_size,
    const void* search,
    size_t search_size) {
  return T_MemSpn(void)(data, data_size, search, search_size);
}

size_t WMemSpn(
    const wchar_t* data,
    size_t data_size,
    const wchar_t* search,
    size_t search_size) {
  return T_MemSpn(wchar_t)(data, data_size, search, search_size);
}

void* MemStr(
    const void* data, size_t data_size, const void* sub, size_t sub_size) {
  return T_MemStr(void)(data, data_size, sub, sub_size);
}

wchar_t* WMemStr(
    const wchar_t* data,
    size_t data_size,
    const wchar_t* sub,
    size_t sub_size) {
  return T_MemStr(wchar_t)(data, data_size, sub, sub_size);
}
