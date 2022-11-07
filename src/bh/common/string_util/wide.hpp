/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2022  SlashDiablo Community
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

#ifndef BH_COMMON_STRING_UTIL_WIDE_HPP_
#define BH_COMMON_STRING_UTIL_WIDE_HPP_

#include <uchar.h>
#include <wchar.h>

#include <locale>
#include <string>
#include <string_view>

namespace bh::common::string_util::wide {

/**
 * Returns a null-terminated wide string converted from a
 * null-terminated UTF-8 string.
 */
template <
    typename CharTraits = std::char_traits<char>,
    typename WCharTraits = std::char_traits<wchar_t>,
    typename WAllocator = std::allocator<wchar_t>>
std::basic_string<wchar_t, WCharTraits, WAllocator> FromUtf8(
    std::basic_string_view<char, CharTraits> src) {
  using CodecvtChar16toChar8 = std::codecvt<char16_t, char8_t, mbstate_t>;

  std::basic_string<wchar_t, WCharTraits, WAllocator> dest(
      src.length(), '\0');

  const CodecvtChar16toChar8& converter =
      std::use_facet<CodecvtChar16toChar8>(std::locale());

  mbstate_t state = { 0 };
  const char* from_next;
  wchar_t* to_next;
  converter.in(
      state,
      reinterpret_cast<const char8_t*>(&src.front()),
      reinterpret_cast<const char8_t*>(&src.back() + 1),
      reinterpret_cast<const char8_t*&>(from_next),
      reinterpret_cast<char16_t*>(&dest.front()),
      reinterpret_cast<char16_t*>(&dest.back() + 1),
      reinterpret_cast<char16_t*&>(to_next));
  dest.resize(to_next - &dest.front());

  return dest;
}

/**
 * Returns a null-terminated wide string converted from a
 * null-terminated UTF-8 string.
 */
template <
    typename CharTraits = std::char_traits<char>,
    typename WCharTraits = std::char_traits<wchar_t>,
    typename WAllocator = std::allocator<wchar_t>>
inline std::basic_string<wchar_t, WCharTraits, WAllocator> FromUtf8(
    const char* src) {
  return FromUtf8(std::basic_string_view<char, CharTraits>(src));
}

/**
 * Returns a null-terminated wide string converted from a
 * null-terminated UTF-8 string.
 */
template <
    typename CharTraits = std::char_traits<char>,
    typename Allocator = std::allocator<char>,
    typename WCharTraits = std::char_traits<wchar_t>,
    typename WAllocator = std::allocator<wchar_t>>
inline std::basic_string<wchar_t, WCharTraits, WAllocator> FromUtf8(
    const std::basic_string<char, CharTraits, Allocator>& src) {
  return FromUtf8(std::basic_string_view<char, CharTraits>(src));
}

/**
 * Returns a null-terminated UTF-8 string converted from a
 * null-terminated wide string.
 */
template <
    typename CharTraits = std::char_traits<char>,
    typename Allocator = std::allocator<char>,
    typename WCharTraits = std::char_traits<wchar_t>>
inline std::basic_string<char, CharTraits, Allocator> ToUtf8(
    std::basic_string_view<wchar_t, WCharTraits> src) {
  using CodecvtChar16toChar8 = std::codecvt<char16_t, char8_t, mbstate_t>;

  std::basic_string<char, CharTraits, Allocator> dest(
      src.length() * 3, '\0');

  const CodecvtChar16toChar8& converter =
      std::use_facet<CodecvtChar16toChar8>(std::locale());

  mbstate_t state = { 0 };
  const wchar_t* from_next;
  char* to_next;
  converter.out(
      state,
      reinterpret_cast<const char16_t*>(&src.front()),
      reinterpret_cast<const char16_t*>(&src.back() + 1),
      reinterpret_cast<const char16_t*&>(from_next),
      reinterpret_cast<char8_t*>(&dest.front()),
      reinterpret_cast<char8_t*>(&dest.back() + 1),
      reinterpret_cast<char8_t*&>(to_next));
  dest.resize(to_next - &dest.front());

  return dest;
}

/**
 * Returns a null-terminated UTF-8 string converted from a
 * null-terminated wide string.
 */
template <
    typename CharTraits = std::char_traits<char>,
    typename Allocator = std::allocator<char>,
    typename WCharTraits = std::char_traits<wchar_t>>
inline std::basic_string<char, CharTraits, Allocator> ToUtf8(
    const wchar_t* src) {
  return ToUtf8(std::basic_string_view<wchar_t, WCharTraits>(src));
}

/**
 * Returns a null-terminated UTF-8 string converted from a
 * null-terminated wide string.
 */
template <
    typename CharTraits = std::char_traits<char>,
    typename Allocator = std::allocator<char>,
    typename WCharTraits = std::char_traits<wchar_t>,
    typename WAllocator = std::allocator<wchar_t>>
inline std::basic_string<char, CharTraits, Allocator> ToUtf8(
    const std::basic_string<wchar_t, WCharTraits, WAllocator>& src) {
  return ToUtf8(std::basic_string_view<wchar_t, WCharTraits>(src));
}

}  // namespace bh::common::string_util::wide

#endif  // BH_COMMON_STRING_UTIL_WIDE_HPP_
