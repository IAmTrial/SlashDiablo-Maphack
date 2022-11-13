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

#include "bh/d2/game/exe/version.hpp"

#include <assert.h>
#include <stddef.h>
#include <windows.h>

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/game/exe/path.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::game::exe::version {
namespace {

using ::bh::common::logging::Logger;
using ::bh::global::GetFileLogger;

namespace game = ::bh::d2::game;

using DisplayVersionNameTableEntry = std::pair<Version, std::wstring_view>;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

static void* InitFileVersionInfo() {
  const wchar_t* game_path = game::exe::GetAbsolutePath().data();

  DWORD unused;
  DWORD file_version_info_size = GetFileVersionInfoSizeW(game_path, &unused);
  if (file_version_info_size == 0) {
    GetLogger().Fatal(
        __LINE__,
        "GetFileVersionInfoSizeW failed with error code 0x{:X}.",
        GetLastError());
    return nullptr;
  }

  auto file_version_info =
      std::make_unique<unsigned char[]>(file_version_info_size);

  DWORD ignored = 0;
  BOOL is_file_version_success =
      GetFileVersionInfoW(
          game_path,
          ignored,
          file_version_info_size,
          file_version_info.get());
  if (!is_file_version_success) {
    GetLogger().Fatal(
        __LINE__,
        "GetFileVersionInfoW failed with error code 0x{:X}.",
        GetLastError());
    return nullptr;
  }

  return file_version_info.release();
}

static VS_FIXEDFILEINFO* GetFixedFileVersionInfo(void* file_version_info) {
  VS_FIXEDFILEINFO* fixed_file_info;

  unsigned int fixed_file_info_size;
  BOOL is_ver_query_value_success =
      VerQueryValueW(
          file_version_info,
          L"\\",
          reinterpret_cast<void**>(&fixed_file_info),
          &fixed_file_info_size);
  if (!is_ver_query_value_success) {
    GetLogger().Fatal(
        __LINE__,
        "VerQueryValueW failed with error code 0x{:X}",
        GetLastError());
    return nullptr;
  }

  return fixed_file_info;
}

static Version GetFixedFileVersion(
    const VS_FIXEDFILEINFO& fixed_file_info) {
  using FileVersion = std::tuple<WORD, WORD, WORD, WORD>;
  using FixedFileVersionTableEntry = std::pair<FileVersion, Version>;

  static constexpr std::array kFixedFileVersionTable =
      std::to_array<FixedFileVersionTableEntry>({
          { { 1, 0, 13, 60 }, Version::k1_13c },
          { { 1, 0, 13, 64 }, Version::k1_13d },
          { { 1, 14, 3, 71 }, Version::k1_14d },
      });
  static_assert(std::ranges::is_sorted(kFixedFileVersionTable));

  auto fixed_file_version =
      std::make_tuple(
          HIWORD(fixed_file_info.dwFileVersionMS),
          LOWORD(fixed_file_info.dwFileVersionMS),
          HIWORD(fixed_file_info.dwFileVersionLS),
          LOWORD(fixed_file_info.dwFileVersionLS));

  auto search_result =
      std::ranges::equal_range(
          kFixedFileVersionTable,
          fixed_file_version,
          {},
          [] (const FixedFileVersionTableEntry& entry) {
            return entry.first;
          });
  if (search_result.empty()) {
    GetLogger().Fatal(
        __LINE__,
        "Unhandled fixed file version with value {}.{}.{}.{}",
        std::get<0>(fixed_file_version),
        std::get<1>(fixed_file_version),
        std::get<2>(fixed_file_version),
        std::get<3>(fixed_file_version));
    return Version::kUnspecified;
  }

  return search_result.front().second;
}

static void* GetGlobalFileVersionInfo() {
  static void* file_version_info = InitFileVersionInfo();
  return file_version_info;
}

static const VS_FIXEDFILEINFO& GetGlobalFixedFileVersionInfo() {
  static VS_FIXEDFILEINFO* fixed_file_info =
      GetFixedFileVersionInfo(GetGlobalFileVersionInfo());
  return *fixed_file_info;
}

}  // namespace

Version GetRunning() {
  static Version version =
      GetFixedFileVersion(GetGlobalFixedFileVersionInfo());
  return version;
}

std::wstring_view GetDisplayName(Version version) {
  switch (version) {
    case Version::k1_13c: {
      static constexpr std::wstring_view kDisplayName = L"1.13c";
      return kDisplayName;
    }

    case Version::k1_13d: {
      static constexpr std::wstring_view kDisplayName = L"1.13d";
      return kDisplayName;
    }

    case Version::k1_14d: {
      static constexpr std::wstring_view kDisplayName = L"1.14d";
      return kDisplayName;
    }
  }

  // This should never happen.
  GetLogger().Fatal(
      __LINE__,
      "Unhandled Version with value {:d}",
      static_cast<int>(version));
  return L"";
}

}  // namespace bh::d2::game::exe::version
