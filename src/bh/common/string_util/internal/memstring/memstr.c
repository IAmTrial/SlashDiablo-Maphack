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

#include "bh/common/string_util/internal/memstring/memstr.h"

#include <wchar.h>

/**
 * External
 */

void* T_MemStr(void)(
    const void* data, size_t data_size, const void* sub, size_t sub_size) {
  return T_MemStr(char)(data, data_size, sub, sub_size);
}

#define T_CHAR char
#include "bh/common/string_util/internal/memstring/memstr_template.h"

#define T_CHAR wchar_t
#include "bh/common/string_util/internal/memstring/memstr_template.h"
