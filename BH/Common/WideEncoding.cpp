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

static std::wstring MultiByteStrToWideStr(
    std::string_view src, UINT code_page) {
  size_t required_length = MultiByteToWideChar(
      code_page,
      MB_ERR_INVALID_CHARS,
      src.data(),
      src.length(),
      nullptr,
      0);
  if (required_length == 0) {
    GetLogger() << "[WARNING] "
        << "MultiByteToWideChar failed with error code 0x"
        << std::hex
        << GetLastError()
        << " for code page "
        << code_page
        << std::endl;
  }

  std::wstring dest(required_length, L'\0');
  int num_characters_written = MultiByteToWideChar(
      code_page,
      MB_ERR_INVALID_CHARS,
      src.data(),
      src.length(),
      dest.data(),
      required_length);
  if (num_characters_written == 0) {
    GetLogger() << "[WARNING] "
        << "MultiByteToWideChar failed with error code 0x"
        << std::hex
        << GetLastError()
        << " for code page "
        << code_page
        << std::endl;
  }

  return dest;
}

static std::string WideStrToMultiByteStr(
    std::wstring_view src, UINT code_page) {
  int required_length = WideCharToMultiByte(
      code_page,
      WC_ERR_INVALID_CHARS,
      src.data(),
      src.length(),
      nullptr,
      0,
      nullptr,
      nullptr);
  if (required_length == 0) {
    GetLogger() << "[WARNING] "
        << "WideCharToMultiByte failed with error code 0x"
        << std::hex
        << GetLastError()
        << " for code page "
        << code_page
        << std::endl;
  }

  std::string dest(required_length, '\0');
  int num_characters_written = WideCharToMultiByte(
      code_page,
      WC_ERR_INVALID_CHARS,
      src.data(),
      src.length(),
      dest.data(),
      required_length,
      nullptr,
      nullptr);
  if (num_characters_written == 0) {
    GetLogger() << "[WARNING] "
        << "WideCharToMultiByte failed with error code 0x"
        << std::hex
        << GetLastError()
        << " for code page "
        << code_page
        << std::endl;
  }

  return dest;
}

} // namespace

std::wstring LocalToWide(std::string_view local_str) {
  return MultiByteStrToWideStr(local_str, CP_ACP);
}

std::wstring Utf8ToWide(std::string_view utf8_str) {
  return MultiByteStrToWideStr(utf8_str, CP_UTF8);
}

std::string WideToLocal(std::wstring_view wide_str) {
  return WideStrToMultiByteStr(wide_str, CP_ACP);
}

std::string WideToUtf8(std::wstring_view wide_str) {
  return WideStrToMultiByteStr(wide_str, CP_UTF8);
}

} // namespace common::wide
