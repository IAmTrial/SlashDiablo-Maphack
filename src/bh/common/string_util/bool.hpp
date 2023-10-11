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

/**
 * BH
 * Copyright (C) 2011  McGod
 *
 * All rights reserved.
 */

#ifndef BH_COMMON_STRING_UTIL_BOOL_HPP_
#define BH_COMMON_STRING_UTIL_BOOL_HPP_

#include <optional>
#include <string>
#include <string_view>

#include "bh/common/string_util/ascii.hpp"

namespace bh::common::string_util {

/**
 * Returns true if the specified string is equal to "true" via
 * case-insensitive comparison. Returns false if the same comparison
 * applies to "false". Otherwise, returns nullopt.
 */
template <typename CharT>
inline constexpr std::optional<bool> ToBool(
    std::basic_string_view<CharT> str) {
  constexpr CharT kTrueStr[] = {
    CharT('t'), CharT('r'), CharT('u'), CharT('e'), CharT('\0')
  };
  constexpr CharT kFalseStr[] = {
    CharT('f'), CharT('a'), CharT('l'), CharT('s'), CharT('e'), CharT('\0')
  };
  constexpr std::char_traits<CharT> ct;

  if (str.length() > std::char_traits<CharT>::length(kFalseStr)) {
    return std::nullopt;
  }

  std::basic_string<CharT> lower_str = ascii::ToLower(str);
  if (lower_str == kTrueStr) {
    return std::make_optional(true);
  } else if (lower_str == kFalseStr) {
    return std::make_optional(false);
  }

  return std::nullopt;
}

/**
 * Returns true if the specified string is equal to "true" via
 * case-insensitive comparison. Returns false if the same comparison
 * applies to "false". Otherwise, returns nullopt.
 */
template <typename CharT>
inline constexpr std::optional<bool> ToBool(const CharT* str) {
  return ToBool(std::basic_string_view<CharT>(str));
}

/**
 * Returns true if the specified string is equal to "true" via
 * case-insensitive comparison. Returns false if the same comparison
 * applies to "false". Otherwise, returns nullopt.
 */
template <typename CharT>
inline constexpr std::optional<bool> ToBool(
    const std::basic_string<CharT>& str) {
  return ToBool(std::basic_string_view<CharT>(str.c_str(), str.length()));
}

}  // namespace bh::common::string_util

#endif  // BH_COMMON_STRING_UTIL_BOOL_HPP_
