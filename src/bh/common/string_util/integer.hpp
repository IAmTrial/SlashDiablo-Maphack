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

#ifndef BH_COMMON_STRING_UTIL_INTEGER_HPP_
#define BH_COMMON_STRING_UTIL_INTEGER_HPP_

#include <assert.h>

#include <optional>
#include <string>
#include <string_view>

namespace bh::common::string_util {

/**
 * Returns an integer value converted from a character. If the
 * character is not a number, or cannot be represented with the
 * integer type, then nullopt is returned instead.
 */
template <typename IntT, typename CharT>
inline constexpr std::optional<IntT> ToIntegerFromDigit(CharT ch, int base) {
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

/**
 * Determines the integer-base encoding that matches the contents of
 * the string's prefix. If the integer-base encoding could not be
 * detected, then the function returns nullopt.
 *
 * Only supports detection of octal, decimal, and hexadecimal.
 */
template <typename CharT>
inline constexpr std::optional<int> GetIntegerBaseFromPrefix(
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

/**
 * Determines the integer-base encoding that matches the contents of
 * the string's prefix. If the integer-base encoding could not be
 * detected, then the function returns nullopt.
 *
 * Only supports detection of octal, decimal, and hexadecimal.
 */
template <typename CharT>
inline constexpr std::optional<int> GetIntegerBaseFromPrefix(
    const CharT* str) {
  return GetIntegerBaseFromPrefix<CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Determines the integer-base encoding that matches the contents of
 * the string's prefix. If the integer-base encoding could not be
 * detected, then the function returns nullopt.
 *
 * Only supports detection of octal, decimal, and hexadecimal.
 */
template <typename CharT>
inline constexpr std::optional<int> GetIntegerBaseFromPrefix(
    const std::basic_string<CharT>& str) {
  return GetIntegerBaseFromPrefix<CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Returns true if the specified character is representable as a
 * number with the specified integer-base encoding.
 *
 * Only the values in the interval [1, 36] are valid for base. The
 * function will always return false if an invalid base is specified.
 */
template <typename CharT>
inline constexpr bool IsDigitOfBase(CharT ch, int base) {
  return ToIntegerFromDigit<unsigned char>(ch, base).has_value();
}

/**
 * Returns true if the specified character is representable as a
 * decimal number.
 */
template <typename CharT>
inline constexpr bool IsDecimalDigit(CharT ch) {
  return IsDigitOfBase<CharT>(ch, 10);
}

/**
 * Returns true if the specified character is representable as a
 * hexadecimal number.
 */
template <typename CharT>
inline constexpr bool IsHexDigit(CharT ch) {
  return IsDigitOfBase<CharT>(ch, 16);
}

/**
 * Returns true if the specified character is representable as an
 * octal number.
 */
template <typename CharT>
inline constexpr bool IsOctalDigit(CharT ch) {
  return IsDigitOfBase<CharT>(ch, 8);
}

/**
 * Determines the integer-base encoding that matches the contents of
 * the string. If the integer-base encoding could not be detected,
 * then the function returns nullopt.
 *
 * Only supports detection of octal, decimal, and hexadecimal.
 */
template <typename CharT>
inline constexpr std::optional<int> GetIntegerBase(
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

/**
 * Determines the integer-base encoding that matches the contents of
 * the string. If the integer-base encoding could not be detected,
 * then the function returns nullopt.
 *
 * Only supports detection of octal, decimal, and hexadecimal.
 */
template <typename CharT>
inline constexpr std::optional<int> GetIntegerBase(const CharT* str) {
  return GetIntegerBase<CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Determines the integer-base encoding that matches the contents of
 * the string. If the integer-base encoding could not be detected,
 * then the function returns nullopt.
 *
 * Only supports detection of octal, decimal, and hexadecimal.
 */
template <typename CharT>
inline constexpr std::optional<int> GetIntegerBase(
    const std::basic_string<CharT>& str) {
  return GetIntegerBase<CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Returns an integer value converted from a string. If the string is
 * not a number, or cannot be represented with the interger type, then
 * nullopt is returned instead.
 */
template <typename IntT, typename CharT>
inline constexpr std::optional<IntT> ToInteger(
    std::basic_string_view<CharT> str) {
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

/**
 * Returns an integer value converted from a string. If the string is
 * not a number, or cannot be represented with the interger type, then
 * nullopt is returned instead.
 */
template <typename IntT, typename CharT>
inline constexpr std::optional<IntT> ToInteger(const CharT* str) {
  return ToInteger<IntT, CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Returns an integer value converted from a string. If the string is
 * not a number, or cannot be represented with the interger type, then
 * nullopt is returned instead.
 */
template <typename IntT, typename CharT>
inline constexpr std::optional<IntT> ToInteger(
    const std::basic_string<CharT>& str) {
  return ToInteger<IntT, CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Returns true if the specified string is representable as a decimal
 * number with the specified integer type.
 */
template <typename IntT, typename CharT>
inline constexpr bool IsDecimal(std::basic_string_view<CharT> str) {
  std::optional<int> base_optional = GetIntegerBase<CharT>(str);
  if (base_optional != 10) {
    return false;
  }

  return ToInteger<IntT, CharT>(str).has_value();
}

/**
 * Returns true if the specified string is representable as a decimal
 * number with the specified integer type.
 */
template <typename IntT, typename CharT>
inline constexpr bool IsDecimal(const CharT* str) {
  return IsDecimal<IntT, CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Returns true if the specified string is representable as a decimal
 * number with the specified integer type.
 */
template <typename IntT, typename CharT>
inline constexpr bool IsDecimal(const std::basic_string<CharT>& str) {
  return IsDecimal<IntT, CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Returns true if the specified string is representable as a
 * hexadecimal number with the specified integer type.
 */
template <typename IntT, typename CharT>
inline constexpr bool IsHex(std::basic_string_view<CharT> str) {
  std::optional<int> base_optional = GetIntegerBase<CharT>(str);
  if (base_optional != 16) {
    return false;
  }

  return ToInteger<IntT, CharT>(str).has_value();
}

/**
 * Returns true if the specified string is representable as a
 * hexadecimal number with the specified integer type.
 */
template <typename IntT, typename CharT>
inline constexpr bool IsHex(const CharT* str) {
  return IsHex<IntT, CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Returns true if the specified string is representable as a
 * hexadecimal number with the specified integer type.
 */
template <typename IntT, typename CharT>
inline constexpr bool IsHex(const std::basic_string<CharT>& str) {
  return IsHex<IntT, CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Returns true if the specified string is representable as an octal
 * number with the specified integer type.
 */
template <typename IntT, typename CharT>
inline constexpr bool IsOctal(std::basic_string_view<CharT> str) {
  std::optional<int> base_optional = GetIntegerBase<CharT>(str);
  if (base_optional != 8) {
    return false;
  }

  return ToInteger<IntT, CharT>(str).has_value();
}

/**
 * Returns true if the specified string is representable as an octal
 * number with the specified integer type.
 */
template <typename IntT, typename CharT>
inline constexpr bool IsOctal(const CharT* str) {
  return IsOctal<IntT, CharT>(std::basic_string_view<CharT>(str));
}

/**
 * Returns true if the specified string is representable as an octal
 * number with the specified integer type.
 */
template <typename IntT, typename CharT>
inline constexpr bool IsOctal(const std::basic_string<CharT>& str) {
  return IsOctal<IntT, CharT>(std::basic_string_view<CharT>(str));
}

}  // namespace bh::common::string_util

#endif  // BH_COMMON_STRING_UTIL_INTEGER_HPP_
