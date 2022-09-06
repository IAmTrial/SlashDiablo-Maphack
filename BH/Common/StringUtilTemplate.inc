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

#include <assert.h>

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
std::optional<int> GetIntegerBaseFromPrefix(
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
std::optional<int> GetIntegerBase(
    const std::basic_string<CharT>& str) {
  return GetIntegerBase<CharT>(std::basic_string_view<CharT>(str));
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
