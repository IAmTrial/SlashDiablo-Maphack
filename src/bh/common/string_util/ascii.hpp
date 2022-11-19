/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2022  SlashDiablo Team
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

#ifndef BH_COMMON_STRING_UTIL_ASCII_HPP_
#define BH_COMMON_STRING_UTIL_ASCII_HPP_

#include <string>
#include <string_view>

namespace bh::common::string_util::ascii {

/**
 * Returns the lowercase character for a specified character. If the
 * character is not an uppercase character for 7-bit ASCII, then the
 * function returns the unmodified character.
 */
template <typename CharT>
inline constexpr CharT ToLowerChar(CharT ch) {
  if (ch < CharT('A') || ch > CharT('Z')) {
    return ch;
  }

  constexpr auto kLowerToUpperDiff = CharT('a') - CharT('A');
  return ch + kLowerToUpperDiff;
}

/**
 * Returns a copy of the specified characters with all uppercase 7-bit
 * ASCII characters converted to lowercase.
 */
template <typename CharT>
inline constexpr std::basic_string<CharT> ToLower(
    std::basic_string_view<CharT> str) {
  std::basic_string<CharT> lower_str(str.cbegin(), str.cend());
  for (CharT& ch : lower_str) {
    ch = ToLowerChar(ch);
  }

  return lower_str;
}

/**
 * Returns a copy of the specified characters with all uppercase 7-bit
 * ASCII characters converted to lowercase.
 */
template <typename CharT>
inline constexpr std::basic_string<CharT> ToLower(const CharT* str) {
  return ToLower(std::basic_string_view<CharT>(str));
}

/**
 * Returns a copy of the specified characters with all uppercase 7-bit
 * ASCII characters converted to lowercase.
 */
template <typename CharT>
inline constexpr std::basic_string<CharT> ToLower(
    const std::basic_string<CharT>& str) {
  return ToLower(std::basic_string_view<CharT>(str));
}

/**
 * Returns the uppercase character for a specified character. If the
 * character is not a lowercase character for 7-bit ASCII, then the
 * function returns the unmodified character.
 */
template <typename CharT>
inline constexpr CharT ToUpperChar(CharT ch) {
  if (ch < CharT('a') || ch > CharT('z')) {
    return ch;
  }

  constexpr auto kLowerToUpperDiff = CharT('a') - CharT('A');
  return ch - kLowerToUpperDiff;
}

/**
 * Returns a copy of the specified characters with all lowercase 7-bit
 * ASCII characters converted to uppercase.
 */
template <typename CharT>
inline constexpr std::basic_string<CharT> ToUpper(
    std::basic_string_view<CharT> str) {
  std::basic_string<CharT> upper_str(str.cbegin(), str.cend());
  for (CharT& ch : upper_str) {
    ch = ToUpperChar(ch);
  }

  return upper_str;
}

/**
 * Returns a copy of the specified characters with all lowercase 7-bit
 * ASCII characters converted to uppercase.
 */
template <typename CharT>
inline constexpr std::basic_string<CharT> ToUpper(const CharT* str) {
  return ToUpper(std::basic_string_view<CharT>(str));
}

/**
 * Returns a copy of the specified characters with all lowercase 7-bit
 * ASCII characters converted to uppercase.
 */
template <typename CharT>
inline constexpr std::basic_string<CharT> ToUpper(
    const std::basic_string<CharT>& str) {
  return ToUpper(std::basic_string_view<CharT>(str));
}

}  // namespace bh::common::string_util::ascii

#endif  // BH_COMMON_STRING_UTIL_ASCII_HPP_
