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

#ifndef BH_COMMON_STRING_UTIL_H_
#define BH_COMMON_STRING_UTIL_H_

#include <string>
#include <string_view>

namespace common::str_util {

/**
 * String transformation functions
 */

/**
 * Returns a view of the specified string, with the starting and
 * ending whitespace removed.
 */
template <typename CharT>
std::basic_string<CharT> Trim(const CharT* str);

/**
 * Returns a view of the specified string, with the starting and
 * ending whitespace removed.
 */
template <typename CharT>
std::basic_string<CharT> Trim(const std::basic_string<CharT>& str);

/**
 * Returns a view of the specified string, with the starting and
 * ending whitespace removed.
 */
template <typename CharT>
std::basic_string<CharT> Trim(std::basic_string_view<CharT> str);

}  // namespace common::str_util

#include "StringUtilTemplate.inc"

#endif  // BH_COMMON_STRING_UTIL_H_
