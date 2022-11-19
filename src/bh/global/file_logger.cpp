/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2022  SlashDiablo Team
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

#include "bh/global/file_logger.hpp"

#include <stddef.h>
#include <wchar.h>
#include <windows.h>
#include <shlwapi.h>

#include <fstream>
#include <memory>
#include <string_view>
#include <unordered_set>
#include <utility>

#include "bh/common/logging/logger.hpp"

namespace bh::global {
namespace {

using ::bh::common::logging::Logger;

static constexpr size_t kBackupLogCount = 3;

static constexpr std::wstring_view kLogFilename = L"BH.log";

static void BackupOldLogFiles() {
  // Shift existing log into backups.
  for (size_t i = kBackupLogCount; i > 1; --i) {
    // This is safe, because the string length will never exceed MAX_PATH.
    std::array<wchar_t, MAX_PATH> old_filename;
    _snwprintf(old_filename.data(), MAX_PATH, L"BH.old.%02u.log", i - 1);

    if (!PathFileExistsW(old_filename.data())) {
      continue;
    }

    // This is safe, because the string length will never exceed MAX_PATH.
    std::array<wchar_t, MAX_PATH> new_filename;
    _snwprintf(new_filename.data(), MAX_PATH, L"BH.old.%02u.log", i);

    MoveFileExW(
        old_filename.data(),
        new_filename.data(),
        MOVEFILE_REPLACE_EXISTING);
  }

  // Shift the main log into the first backup, if it exists.
  if (!PathFileExistsW(kLogFilename.data())) {
    return;
  }

  MoveFileExW(
      kLogFilename.data(),
      L"BH.old.01.log",
      MOVEFILE_REPLACE_EXISTING);
}

static std::shared_ptr<std::ofstream>* InitLogFile() {
  BackupOldLogFiles();

  auto file = std::make_shared<std::ofstream>(kLogFilename.data());
  return new std::shared_ptr(std::move(file));
}

static std::shared_ptr<std::ofstream>& GetLogFile() {
  static auto* log_file = InitLogFile();
  return *log_file;
}

}  // namespace

Logger& GetFileLogger(std::wstring_view src_path) {
  static auto* known_path_views =
      new std::unordered_set<std::wstring_view>();

  // Last error needs to be saved in the case that GetLastError is
  // called immediately after this function.
  DWORD old_last_error = GetLastError();
  Logger& logger = Logger::GetLogger(src_path);
  if (!known_path_views->contains(src_path)) {
    static auto* known_paths = new std::unordered_set<std::wstring>();
    
    logger.AddStream(GetLogFile());
    auto [it, _] = known_paths->insert(std::wstring(src_path));
    known_path_views->insert(*it);
  }
  SetLastError(old_last_error);

  return logger;
}

}  // namespace bh::global
