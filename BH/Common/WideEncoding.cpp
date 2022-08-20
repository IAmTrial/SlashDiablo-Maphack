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

#include "WideEncoding.h"

#include <stddef.h>
#include <windows.h>

#include <fstream>
#include <ios>
#include <ostream>
#include <string>
#include <string_view>

namespace common::wide {
namespace {

static inline constexpr std::wstring_view kLogPath = L"./BH.Common.log";

static std::ofstream& GetLogger() {
  static std::ofstream logger(kLogPath.data(), std::ios_base::app);
  return logger;
}

} // namespace

std::wstring Utf8ToWide(std::string_view utf8_str) {
  size_t required_length = MultiByteToWideChar(
      CP_UTF8,
      MB_ERR_INVALID_CHARS,
      utf8_str.data(),
      utf8_str.length(),
      nullptr,
      0);
  if (required_length == 0) {
    GetLogger() << "MultiByteToWideChar failed with error code 0x"
        << std::hex
        << GetLastError()
        << std::endl;
  }

  std::wstring wide_str(required_length, L'\0');
  int num_characters_written = MultiByteToWideChar(
      CP_UTF8,
      MB_ERR_INVALID_CHARS,
      utf8_str.data(),
      utf8_str.length(),
      wide_str.data(),
      required_length);
  if (num_characters_written == 0) {
    GetLogger() << "MultiByteToWideChar failed with error code 0x"
        << std::hex
        << GetLastError()
        << std::endl;
  }

  return wide_str;
}

/**
 * Converts a wide string into a UTF-8 encoded string.
 */
std::string WideToUtf8(std::wstring_view wide_str) {
  int required_length = WideCharToMultiByte(
      CP_UTF8,
      WC_ERR_INVALID_CHARS,
      wide_str.data(),
      wide_str.length(),
      nullptr,
      0,
      nullptr,
      nullptr);
  if (required_length == 0) {
    GetLogger() << "[SEVERE] "
        << "WideCharToMultiByte failed with error code 0x"
        << std::hex
        << GetLastError()
        << std::endl;
  }

  std::string utf8_str(required_length, '\0');
  int num_characters_written = WideCharToMultiByte(
      CP_UTF8,
      WC_ERR_INVALID_CHARS,
      wide_str.data(),
      wide_str.length(),
      utf8_str.data(),
      required_length,
      nullptr,
      nullptr);
  if (num_characters_written == 0) {
    GetLogger() << "[SEVERE] "
        << "WideCharToMultiByte failed with error code 0x"
        << std::hex
        << GetLastError()
        << std::endl;
  }

  return utf8_str;
}

} // namespace common::wide
