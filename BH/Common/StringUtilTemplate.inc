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

#if !defined(BH_COMMON_STRING_UTIL_H_)
#error This file is not to be directly included by external users.
#endif  // !defined(BH_COMMON_STRING_UTIL_H_)

#ifndef BH_COMMON_STRING_UTIL_TEMPLATE_INC_
#define BH_COMMON_STRING_UTIL_TEMPLATE_INC_

#include <string>
#include <string_view>

namespace common::str_util {

/**
 * String transformation functions
 */

template <typename CharT>
std::basic_string<CharT> Trim(const CharT* str) {
  return Trim<CharT>(std::basic_string_view<CharT>(str));
}

template <typename CharT>
std::basic_string<CharT> Trim(const std::basic_string<CharT>& str) {
  return Trim<CharT>(std::basic_string_view<CharT>(str));
}

template <typename CharT>
std::basic_string<CharT> Trim(std::basic_string_view<CharT> str) {
  using size_type = typename std::basic_string_view<CharT>::size_type;

  static constexpr CharT kWhitespaceChars[] = {
    CharT(' '),
    CharT('\f'),
    CharT('\n'),
    CharT('\r'),
    CharT('\t'),
    CharT('\v'),
    CharT('\0')
  };

  size_type trim_front_index = str.find_first_not_of(kWhitespaceChars);
  if (trim_front_index == std::basic_string_view<CharT>::npos) {
    return std::basic_string<CharT>();
  }

  size_type trim_back_index = str.find_last_not_of(kWhitespaceChars);

  return std::basic_string<CharT>(
      str.substr(trim_front_index, (trim_back_index + 1) - trim_front_index));
}

}  // namespace common::str_util

#endif  // BH_COMMON_STRING_UTIL_TEMPLATE_INC_
