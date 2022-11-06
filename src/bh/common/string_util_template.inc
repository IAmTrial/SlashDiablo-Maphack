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

#if !defined(BH_COMMON_STRING_UTIL_HPP_)
#error This file is not to be directly included by external users.
#endif  // !defined(BH_COMMON_STRING_UTIL_HPP_)

#ifndef BH_COMMON_STRING_UTIL_TEMPLATE_INC_
#define BH_COMMON_STRING_UTIL_TEMPLATE_INC_

#include <assert.h>

#include <limits>
#include <locale>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

/*
 * Unlike the forward declarations, everything constexpr needs to be
 * declared in an upstream to downstream dependency order, otherwise
 * the function will not be recognized as constexpr (due to inline
 * rules).
 *
 * This means that anything with zero dependencies should sit near the
 * top of this file.
 */

namespace common::str_util {
namespace ascii {

template <typename CharT>
constexpr std::basic_string<CharT> ToLower(const CharT* str) {
  return ToLower(std::basic_string_view<CharT>(str));
}

template <typename CharT>
constexpr std::basic_string<CharT> ToLower(const std::basic_string<CharT>& str) {
  return ToLower(std::basic_string_view<CharT>(str));
}

template <typename CharT>
constexpr std::basic_string<CharT> ToLower(std::basic_string_view<CharT> str) {
  std::basic_string<CharT> lower_str(str.cbegin(), str.cend());
  for (CharT& ch : lower_str) {
    ch = ToLowerChar(ch);
  }

  return lower_str;
}

template <typename CharT>
constexpr CharT ToLowerChar(CharT ch) {
  if (ch < CharT('A') || ch > CharT('Z')) {
    return ch;
  }

  constexpr auto kLowerToUpperDiff = CharT('a') - CharT('A');
  return ch + kLowerToUpperDiff;
}

}  // namespace ascii

namespace wide {

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

template <
    typename CharTraits = std::char_traits<char>,
    typename WCharTraits = std::char_traits<wchar_t>,
    typename WAllocator = std::allocator<wchar_t>>
std::basic_string<wchar_t, WCharTraits, WAllocator> FromUtf8(
    const char* src) {
  return FromUtf8(std::basic_string_view<char, CharTraits>(src));
}

template <
    typename CharTraits = std::char_traits<char>,
    typename Allocator = std::allocator<char>,
    typename WCharTraits = std::char_traits<wchar_t>,
    typename WAllocator = std::allocator<wchar_t>>
std::basic_string<wchar_t, WCharTraits, WAllocator> FromUtf8(
    const std::basic_string<char, CharTraits, Allocator>& src) {
  return FromUtf8(std::basic_string_view<char, CharTraits>(src));
}

template <
    typename CharTraits = std::char_traits<char>,
    typename Allocator = std::allocator<char>,
    typename WCharTraits = std::char_traits<wchar_t>>
std::basic_string<char, CharTraits, Allocator> ToUtf8(
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

template <
    typename CharTraits = std::char_traits<char>,
    typename Allocator = std::allocator<char>,
    typename WCharTraits = std::char_traits<wchar_t>>
std::basic_string<char, CharTraits, Allocator> ToUtf8(const wchar_t* src) {
  return ToUtf8(std::basic_string_view<wchar_t, WCharTraits>(src));
}

template <
    typename CharTraits = std::char_traits<char>,
    typename Allocator = std::allocator<char>,
    typename WCharTraits = std::char_traits<wchar_t>,
    typename WAllocator = std::allocator<wchar_t>>
std::basic_string<char, CharTraits, Allocator> ToUtf8(
    const std::basic_string<wchar_t, WCharTraits, WAllocator>& src) {
  return ToUtf8(std::basic_string_view<wchar_t, WCharTraits>(src));
}

}  // namespace wide

/**
 * String-to-type functions
 */

template <typename CharT>
constexpr std::optional<bool> ToBool(const CharT* str) {
  return ToBool(std::basic_string_view<CharT>(str));
}

template <typename CharT>
constexpr std::optional<bool> ToBool(const std::basic_string<CharT>& str) {
  return ToBool(std::basic_string_view<CharT>(str.c_str(), str.length()));
}

template <typename CharT>
constexpr std::optional<bool> ToBool(std::basic_string_view<CharT> str) {
  constexpr CharT kTrueStr[] = {
    CharT('t'), CharT('r'), CharT('u'), CharT('e'), CharT('\0')
  };
  constexpr std::basic_string_view<CharT> kTrueStrView = kTrueStr;
  constexpr CharT kFalseStr[] = {
    CharT('f'), CharT('a'), CharT('l'), CharT('s'), CharT('e'), CharT('\0')
  };
  constexpr std::basic_string_view<CharT> kFalseStrView = kFalseStr;

  if (str.length() > kFalseStrView.length()) {
    return std::nullopt;
  }

  std::basic_string<CharT> lower_str = ascii::ToLower(str);
  if (lower_str == kTrueStrView) {
    return std::make_optional(true);
  } else if (lower_str == kFalseStrView) {
    return std::make_optional(false);
  }

  return std::nullopt;
}

template <typename IntT, typename CharT>
constexpr std::optional<IntT> ToIntegerFromDigit(CharT ch, int base) {
  // Only the values in interval [1, 36] are valid for base.
  if (base < 1 || base > 36) {
    return std::nullopt;
  }

  // This compare of CharT against char is safe, because 7-bit ASCII
  // characters use the same value to represent characters in the
  // 0-128 range. CharT being signed or unsigned does not matter.

  std::optional<IntT> value = std::nullopt;
  if (ch >= CharT('0') && ch <= CharT('9')) {
    value = std::make_optional<IntT>(ch - CharT('0'));
  } else if (ch >= CharT('a') && ch <= CharT('z')) {
    value = std::make_optional<IntT>(ch - CharT('a') + 10);
  } else if (ch >= CharT('A') && ch <= CharT('Z')) {
    value = std::make_optional<IntT>(ch - CharT('A') + 10);
  }

  if (value >= IntT(base)) {
    return std::nullopt;
  }

  return value;
}

template <typename CharT>
constexpr std::optional<int> GetIntegerBaseFromPrefix(
    std::basic_string_view<CharT> str) {
  using size_type = std::basic_string_view<CharT>::size_type;

  if (str.empty()) {
    return std::nullopt;
  }

  // Track the negative sign.
  bool is_negative = (str[0] == CharT('-'));

  size_type start_index = is_negative ? 1 : 0;
  if (str[start_index] == CharT('0')) {
    if (str.length() == start_index + 1) {
      // The string is "0" or "-0", so the prefix is decimal.
      return 10;
    }
    assert(str.length() > start_index + 1);

    if (str[start_index + 1] == CharT('x')
        || str[start_index + 1] == CharT('X')) {
      // The string starts with "0x" or "0X", so the prefix is hexadecimal.
      return 16;
    }

    // The string starts with "0", so the prefix is octal.
    return 8;
  }

  if (str[start_index] >= CharT('1') && str[start_index] <= CharT('9')) {
    // The prefix is decimal.
    return 10;
  }

  return std::nullopt;
}

template <typename CharT>
constexpr std::optional<int> GetIntegerBaseFromPrefix(const CharT* str) {
  return GetIntegerBaseFromPrefix<CharT>(std::basic_string_view<CharT>(str));
}

template <typename CharT>
constexpr std::optional<int> GetIntegerBaseFromPrefix(
    const std::basic_string<CharT>& str) {
  return GetIntegerBaseFromPrefix<CharT>(std::basic_string_view<CharT>(str));
}

template <typename CharT>
constexpr bool IsDigitOfBase(CharT ch, int base) {
  return ToIntegerFromDigit<unsigned char>(ch, base).has_value();
}

template <typename CharT>
constexpr bool IsDecimalDigit(CharT ch) {
  return IsDigitOfBase<CharT>(ch, 10);
}

template <typename CharT>
constexpr bool IsHexDigit(CharT ch) {
  return IsDigitOfBase<CharT>(ch, 16);
}

template <typename CharT>
constexpr bool IsOctalDigit(CharT ch) {
  return IsDigitOfBase<CharT>(ch, 8);
}

template <typename CharT>
constexpr std::optional<int> GetIntegerBase(
    std::basic_string_view<CharT> str) {
  using size_type = std::basic_string_view<CharT>::size_type;

  if (str.empty()) {
    return std::nullopt;
  }

  std::optional<int> prefix_base_optional =
      GetIntegerBaseFromPrefix<CharT>(str);
  if (!prefix_base_optional.has_value()) {
    return std::nullopt;
  }
  int prefix_base = *prefix_base_optional;

  // Make sure our string meets minimum length requirement for the
  // prefix's base.
  size_type start_index = (str[0] == '-') ? 1 : 0;
  switch (prefix_base) {
    case 8: {
      start_index += 1;
      break;
    }

    case 10: {
      break;
    }

    case 16: {
      start_index += 2;
      break;
    }

    default: {
      // This should never happen.
      assert(false);
      return std::nullopt;
    }
  }

  if (str.length() <= start_index) {
    return std::nullopt;
  }

  // Check that all characters match with the prefix's base.
  for (CharT ch : str.substr(start_index)) {
    if (!IsDigitOfBase(ch, prefix_base)) {
      return std::nullopt;
    }
  }

  return prefix_base_optional;
}

template <typename CharT>
constexpr std::optional<int> GetIntegerBase(const CharT* str) {
  return GetIntegerBase<CharT>(std::basic_string_view<CharT>(str));
}

template <typename CharT>
constexpr std::optional<int> GetIntegerBase(
    const std::basic_string<CharT>& str) {
  return GetIntegerBase<CharT>(std::basic_string_view<CharT>(str));
}

template <typename IntT, typename CharT>
constexpr std::optional<IntT> ToInteger(std::basic_string_view<CharT> str) {
  using size_type = std::basic_string_view<CharT>::size_type;
  using numeric_limits = std::numeric_limits<IntT>;

  if (str.empty()) {
    return std::nullopt;
  }

  // Check if the digit is valid for the detected prefix's base. If
  // it is valid, get the value.
  std::optional<int> prefix_base_optional =
      GetIntegerBaseFromPrefix<CharT>(str);
  if (!prefix_base_optional.has_value()) {
    return std::nullopt;
  }
  int prefix_base = *prefix_base_optional;

  // Do not allow unsigned numbers to have negative numbers.
  bool is_negative = (str[0] == '-');
  if constexpr (!numeric_limits::is_signed) {
    // We use std::numeric_limits::is_signed instead of <type_traits>
    // std::is_signed so that custom integer-like types can be used
    // in this function.
    if (is_negative) {
      return std::nullopt;
    }
  }

  size_type start_index = (is_negative) ? 1 : 0;
  switch (prefix_base) {
    case 8: {
      start_index += 1;
      break;
    }

    case 10: {
      break;
    }

    case 16: {
      start_index += 2;
      break;
    }

    default: {
      // This should never happen.
      assert(false);
      return std::nullopt;
    }
  }

  if (str.length() <= start_index) {
    return std::nullopt;
  }

  // Convert the string to the integer type.
  IntT limit =
      is_negative ? numeric_limits::lowest() : numeric_limits::max();
  IntT value = 0;
  for (CharT ch : str.substr(start_index)) {
    // Check if the string has too many digits.
    if ((is_negative && value < (limit / prefix_base))
        || (!is_negative && value > (limit / prefix_base))) {
      return std::nullopt;
    }
    value *= prefix_base;

    // Check if the digit is valid for the detected prefix's base.
    // If it is valid, get the value.
    std::optional<IntT> digit_optional =
        ToIntegerFromDigit<IntT>(ch, prefix_base);
    if (!digit_optional.has_value()) {
      return std::nullopt;
    }
    IntT digit = *digit_optional;

    // Check if add/sub of this digit to the value would go
    // over/under the limit.
    if ((is_negative && value < (limit + digit))
      || (!is_negative && value > (limit - digit))) {
      return std::nullopt;
    }

    value += (is_negative ? -1 : 1) * digit;
  }

  return value;
}

template <typename IntT, typename CharT>
constexpr std::optional<IntT> ToInteger(const CharT* str) {
  return ToInteger<IntT, CharT>(std::basic_string_view<CharT>(str));
}

template <typename IntT, typename CharT>
constexpr std::optional<IntT> ToInteger(
    const std::basic_string<CharT>& str) {
  return ToInteger<IntT, CharT>(std::basic_string_view<CharT>(str));
}

template <typename IntT, typename CharT>
constexpr bool IsDecimal(std::basic_string_view<CharT> str) {
  std::optional<int> base_optional = GetIntegerBase<CharT>(str);
  if (base_optional != 10) {
    return false;
  }

  return ToInteger<IntT, CharT>(str).has_value();
}

template <typename IntT, typename CharT>
constexpr bool IsDecimal(const CharT* str) {
  return IsDecimal<IntT, CharT>(std::basic_string_view<CharT>(str));
}

template <typename IntT, typename CharT>
constexpr bool IsDecimal(const std::basic_string<CharT>& str) {
  return IsDecimal<IntT, CharT>(std::basic_string_view<CharT>(str));
}

template <typename IntT, typename CharT>
constexpr bool IsHex(std::basic_string_view<CharT> str) {
  std::optional<int> base_optional = GetIntegerBase<CharT>(str);
  if (base_optional != 16) {
    return false;
  }

  return ToInteger<IntT, CharT>(str).has_value();
}

template <typename IntT, typename CharT>
constexpr bool IsHex(const CharT* str) {
  return IsHex<IntT, CharT>(std::basic_string_view<CharT>(str));
}

template <typename IntT, typename CharT>
constexpr bool IsHex(const std::basic_string<CharT>& str) {
  return IsHex<IntT, CharT>(std::basic_string_view<CharT>(str));
}

template <typename IntT, typename CharT>
constexpr bool IsOctal(std::basic_string_view<CharT> str) {
  std::optional<int> base_optional = GetIntegerBase<CharT>(str);
  if (base_optional != 8) {
    return false;
  }

  return ToInteger<IntT, CharT>(str).has_value();
}

template <typename IntT, typename CharT>
constexpr bool IsOctal(const CharT* str) {
  return IsOctal<IntT, CharT>(std::basic_string_view<CharT>(str));
}

template <typename IntT, typename CharT>
bool IsOctal(const std::basic_string<CharT>& str) {
  return IsOctal<IntT, CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Non-constexpr functions
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
