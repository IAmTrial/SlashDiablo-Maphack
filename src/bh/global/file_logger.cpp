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
#include <tlhelp32.h>

#include <fstream>
#include <memory>
#include <string_view>
#include <unordered_set>
#include <utility>

#include "bh/common/logging/logger.hpp"
#include "bh/common/string_util/ascii.hpp"
#include "bh/common/windows/handle_closer.hpp"

namespace bh::global {
namespace {

using ::bh::common::logging::Logger;
using ::bh::common::string_util::ascii::ToLower;
using ::bh::common::windows::HandleCloser;

static constexpr size_t kBackupLogCount = 3;
static constexpr size_t kLoggedInstancesCount = 16;

/**
 * Backs up a set of log files. Existing old log files will be shifted
 * by one level. Will not back up logs beyond the backup log limit.
 * 
 * The 0th set is considered the current logs, with format
 * "BH.i%02u.log". Existing backup logs use format
 * "BH.old%02u.i%02u.log".
 */
static void BackupLogFileSet(size_t i_backup) {
  // The last set of old logs should not be backed up.
  if (i_backup >= kBackupLogCount) {
    return;
  }

  // Set up the prefix to the old log filename string. This saves having to
  // constantly write the filename prefix.
  std::array<wchar_t, MAX_PATH> old_filename;

  // This is safe, because the string length will never exceed MAX_PATH.
  size_t old_log_filename_prefix_length = _snwprintf(
      old_filename.data(),
      MAX_PATH,
      (i_backup > 0) ? L"BH.old%02u.i" : L"BH.i",
      i_backup);

  for (size_t i_instance = 0;
      i_instance < kLoggedInstancesCount;
      ++i_instance) {
    // This is safe, because the string length will never exceed MAX_PATH.
    _snwprintf(
        &old_filename[old_log_filename_prefix_length],
        MAX_PATH - old_log_filename_prefix_length,
        L"%02u.log",
        i_instance + 1);

    if (!PathFileExistsW(old_filename.data())) {
      continue;
    }

    // This is safe, because the string length will never exceed MAX_PATH.
    std::array<wchar_t, MAX_PATH> new_filename;
    _snwprintf(
        new_filename.data(),
        MAX_PATH,
        L"BH.old%02u.i%02u.log",
        i_backup + 1,
        i_instance + 1);

    MoveFileExW(
        old_filename.data(),
        new_filename.data(),
        MOVEFILE_REPLACE_EXISTING);
  }
}

static void BackupOldLogFiles() {
  // Shift existing backups by one level and back up the existing
  // logs. This must be done in order of oldest to newest, or else
  // the oldest logs will be overwriten before backing them up.
  for (size_t i = kBackupLogCount; i-- > 0; ) {
    BackupLogFileSet(i);
  }
}

static bool IsGameProcessExist() {
  // Snapshot the processes.
  HANDLE snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (snapshot_handle == INVALID_HANDLE_VALUE) {
    return false;
  }
  HandleCloser snapshot_handle_closer(snapshot_handle);

  // Iterate through all processes to find Game.exe.
  PROCESSENTRY32W process_entry;
  process_entry.dwSize = sizeof(process_entry);

  BOOL process_entry_iterate_succeeded;
  for (process_entry_iterate_succeeded =
          Process32FirstW(snapshot_handle, &process_entry);
      process_entry_iterate_succeeded;
      process_entry_iterate_succeeded =
          Process32NextW(snapshot_handle, &process_entry)) {
    // Skip if the process ID is the same as this process.
    if (process_entry.th32ProcessID == GetCurrentProcessId()) {
      continue;
    }

    // Case insensitive compare for "Game.exe".
    std::wstring lower_filename = ToLower(process_entry.szExeFile);
    if (lower_filename.ends_with(L"game.exe")) {
      return true;
    }
  }

  DWORD last_error = GetLastError();
  if (last_error != ERROR_NO_MORE_FILES ) {
    // This is an error, but there isn't anything that can be done
    // to handle this error.
    return false;
  }

  return false;
}

static void DeleteOldestBackupLogFiles() {
  // This is safe, because the string length will never exceed MAX_PATH.
  for (size_t i = 0; i < kLoggedInstancesCount; ++i) {
    std::array<wchar_t, MAX_PATH> filename;
    _snwprintf(
        filename.data(),
        MAX_PATH,
        L"BH.old%02u.i%02u.log",
        kBackupLogCount,
        i);
    DeleteFileW(filename.data());
  }
}

static void SetUpFileSystem() {
  // If game processes are running, then assume that the filesystem is
  // already set up.
  if (IsGameProcessExist()) {
    return;
  }

  DeleteOldestBackupLogFiles();
  BackupOldLogFiles();
}

static std::wstring_view GetLogFilename() {
  static std::array<wchar_t, MAX_PATH> log_filename =
      []() {
        // Find the first filename that is not occupied by an existing
        // instance.
        static constexpr std::wstring_view kFilenamePrefix = L"BH.i";
        std::array<wchar_t, MAX_PATH> filename;
        kFilenamePrefix.copy(filename.data(), kFilenamePrefix.length());

        for (size_t i = 0; i++ < kLoggedInstancesCount; ) {
          // This is safe, because the string length will never exceed MAX_PATH.
          _snwprintf(
              &filename[kFilenamePrefix.length()],
              MAX_PATH - kFilenamePrefix.length(),
              L"%02u.log",
              i);
          if (PathFileExistsW(filename.data())) {
            continue;
          }

          return filename;
        }

        filename[0] = '\0';
        return filename;
      }();
  
  return std::wstring_view(log_filename.cbegin(), log_filename.cend());
}

static std::shared_ptr<std::ofstream>* InitLogFile() {
  SetUpFileSystem();

  auto file = std::make_shared<std::ofstream>(GetLogFilename().data());
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
