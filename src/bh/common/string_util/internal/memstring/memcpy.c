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

#include "bh/common/string_util/internal/memstring/memcpy.h"

#include <stddef.h>
#include <string.h>
#include <wchar.h>

/**
 * External
 */

void* T_MemCpy(void)(void* dest, const void* src, size_t count) {
  return T_MemCpy(char)(dest, src, count);
}

char* T_MemCpy(char)(char* dest, const char* src, size_t count) {
  return memcpy(dest, src, count);
}

wchar_t* T_MemCpy(wchar_t)(wchar_t* dest, const wchar_t* src, size_t count) {
  return wmemcpy(dest, src, count);
}
