/**
 * SlashDiablo Maphack
 * Copyright (C) SlashDiablo Community
 *
 * BH
 * Copyright 2011 (C) McGod
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

#ifndef BH_COMMON_WIDE_ENCODING_H_
#define BH_COMMON_WIDE_ENCODING_H_

#include <string>
#include <string_view>

namespace common::wide {

/**
 * Converts a UTF-8 encoded string into a wide string.
 */
std::wstring Utf8ToWide(std::string_view utf8_str);

/**
 * Converts a wide string into a UTF-8 encoded string.
 */
std::string WideToUtf8(std::wstring_view wide_str);

} // namespace common::wide

#endif // BH_COMMON_WIDE_ENCODING_H_
