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

#include "bh/d2/dll/handle.hpp"

#include <windows.h>

#include <array>
#include <algorithm>
#include <unordered_map>
#include <utility>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/dll/dll.hpp"
#include "bh/d2/dll/internal/relative_path.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::dll {
namespace {

using ::bh::common::logging::Logger;
using ::bh::global::GetFileLogger;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

static std::array<std::pair<Dll, HMODULE>, kDlls.size()> InitHandlesTable() {
  /*
   * The handles table is not initialized with all library handles,
   * because certain ones require a specific condition to be loaded
   * (for example, D2Glide.dll).
   */
  std::array<std::pair<Dll, HMODULE>, kDlls.size()> handles_table = { };
  std::ranges::transform(
      kDlls,
      handles_table.begin(),
      [] (Dll dll) {
        return std::pair<Dll, HMODULE>(dll, nullptr);
      });

  return handles_table;
}

static HMODULE InitModule(Dll dll) {
  std::wstring_view relative_path = internal::GetRelativePath(dll);
  HMODULE handle = LoadLibraryW(relative_path.data());
  if (handle == nullptr) {
    GetLogger().Fatal(
        __LINE__,
        "LoadLibraryW failed with error code 0x{:X}.",
        GetLastError());
    return nullptr;
  }

  return handle;
}

}  // namespace

HMODULE GetHandle(Dll dll) {
  static std::array handles_table = InitHandlesTable();
  auto search_result =
      std::ranges::equal_range(
          handles_table,
          dll,
          {},
          [] (const auto& entry) {
            return entry.first;
          });

  // No result, but this should never happen.
  if (search_result.empty()) {
    GetLogger().Fatal(
        __LINE__,
        "Unhandled Dll with value {:d}",
        static_cast<int>(dll));
    return nullptr;
  }

  // Load the module to the table, if it isn't there.
  HMODULE& handle = search_result.front().second;
  if (handle == nullptr) {
    handle = InitModule(dll);
  }

  return handle;
}

}  // namespace bh::d2::dll