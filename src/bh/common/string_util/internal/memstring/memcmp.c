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

#include "bh/common/string_util/internal/memstring/memcmp.h"

#include <stddef.h>
#include <string.h>
#include <wchar.h>

/**
 * External
 */

int T_MemCmp(void)(const void* left, const void* right, size_t count) {
  return T_MemCmp(char)(left, right, count);
}

int T_MemCmp(char)(const char* left, const char* right, size_t count) {
  return memcmp(left, right, count);
}

int T_MemCmp(wchar_t)(
    const wchar_t* left, const wchar_t* right, size_t count) {
  return wmemcmp(left, right, count);
}
