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

#include "bh/d2/dll/path.hpp"

#include <wchar.h>
#include <windows.h>

#include <algorithm>
#include <array>
#include <string_view>
#include <utility>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/dll/dll.hpp"
#include "bh/d2/dll/handle.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::dll {
namespace {

using ::bh::common::logging::Logger;
using ::bh::global::GetFileLogger;

using PathType = std::array<wchar_t, MAX_PATH>;
using PathViewsTableEntry = std::pair<Dll, std::wstring_view>;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

static std::array<std::pair<Dll, PathType>, kDlls.size()> InitPathTable() {
  /*
   * The path table is not initialized with all library paths,
   * because certain ones require a specific condition to be loaded
   * (for example, D2Glide.dll).
   */
  std::array<std::pair<Dll, PathType>, kDlls.size()> path_table;
  std::ranges::transform(
      kDlls,
      path_table.begin(),
      [] (Dll dll) {
        return std::pair<Dll, PathType>(dll, PathType());
      });

  return path_table;
}

static PathType InitPath(Dll dll) {
  HMODULE handle = GetHandle(dll);

  std::array<wchar_t, MAX_PATH> path;
  DWORD chars_written = GetModuleFileNameW(handle, path.data(), path.size());
  if (chars_written == 0) {
    GetLogger().Fatal(
        __LINE__,
        "GetModuleFileNameW failed with error code 0x{:X}.",
        GetLastError());
    return path;
  }

  return path;
}

static std::array<wchar_t, MAX_PATH>& GetNativeAbsolutePath(Dll dll) {
  static auto path_table = InitPathTable();

  auto search_result =
      std::ranges::equal_range(
          path_table,
          dll,
          {},
          [] (const auto& entry) {
            return entry.first;
          });

  // Checks if a result was not found, which should not happen.
  if (search_result.empty()) {
    GetLogger().Fatal(
        __LINE__,
        "Unhandled Dll with value {:d}.",
        static_cast<int>(dll));
    return PathType();
  }

  // Init the path, if is hasn't yet.
  PathType& path = search_result.front().second;
  if (path[0] == L'\0') {
    path = InitPath(dll);
  }

  return path;
}

static std::array<PathViewsTableEntry, kDlls.size()> InitPathViewTable() {
  std::array<PathViewsTableEntry, kDlls.size()> path_views_table;
  std::ranges::transform(
      kDlls,
      path_views_table.begin(),
      [] (Dll dll) {
        return PathViewsTableEntry(dll, std::wstring_view());
      });

  return path_views_table;
}

}  // namespace

std::wstring_view GetAbsolutePath(Dll dll) {
  static std::array<PathViewsTableEntry, kDlls.size()> path_views_table =
      InitPathViewTable();

  auto search_result =
      std::ranges::equal_range(
          path_views_table,
          dll,
          {},
          [] (const PathViewsTableEntry& entry) {
            return entry.first;
          });

  // Checks if a result was not found, which should not happen.
  if (search_result.empty()) {
    GetLogger().Fatal(
        __LINE__,
        "Unhandled Dll with value {:d}.",
        static_cast<int>(dll));
    return std::wstring_view();
  }

  // Init the view, if is hasn't yet.
  std::wstring_view& path_view = search_result.front().second;
  if (path_view.empty()) {
    path_view = GetNativeAbsolutePath(dll).data();
  }

  return path_view;
}

}  // namespace bh::d2::dll
