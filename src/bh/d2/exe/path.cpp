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

#include "bh/d2/exe/path.hpp"

#include <windows.h>

#include <array>
#include <string_view>

#include "bh/common/logging/logger.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::exe {
namespace {

using ::bh::common::logging::Logger;
using ::bh::global::GetFileLogger;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

static std::array<wchar_t, MAX_PATH> InitAbsolutePath() {
  std::array<wchar_t, MAX_PATH> path;

  DWORD chars_written =
      GetModuleFileNameW(nullptr, path.data(), MAX_PATH);
  if (chars_written == 0) {
    GetLogger().Fatal(
        __LINE__,
        "GetModuleFileNameW failed with error code 0x{:X}.",
        GetLastError());
    return path;
  }

  return path;
}

}  // namespace

std::wstring_view GetAbsolutePath() {
  static std::array<wchar_t, MAX_PATH> path = InitAbsolutePath();
  static std::wstring_view path_view = path.data();
  return path_view;
}

}  // namespace bh::d2::exe
