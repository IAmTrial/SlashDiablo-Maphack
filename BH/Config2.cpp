/**
 * SlashDiablo Maphack
 * Copyright (C) SlashDiablo Community
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

#include "Config2.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include <array>
#include <ios>
#include <filesystem>
#include <fstream>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

namespace config {
namespace {

static constexpr inline std::wstring_view kLogPath =
    L"./config_read_write.log";
static constexpr inline int kLineCapacity = 2048;

// TODO (Mir Drualga): This is REALLY bad practice, but I do this so
// that there is a logger that can be used at the file scope.
static std::ofstream* logger;

} // namespace

Config::Config(std::filesystem::path path) noexcept :
    path_(std::move(path)) {}

/*
 * This is a reminder that the parsing of any user input should
 * always be treated as unsafe. Do not allow arbitrary memory usage
 * and always check for invalid states.
 */
bool Config::ReadFile() {
  // TODO (Mir Drualga): This is REALLY bad practice, but I do this so
  // that there is a logger that can be used at the file scope.
  std::ofstream actual_logger(kLogPath, std::ios_base::app);
  logger = &actual_logger;

  // Perform basic tests in order to safely open the config file.
  std::filesystem::file_status file_status = std::filesystem::status(path_);
  if (!std::filesystem::exists(file_status)) {
    *logger << "[WARNING] Path " << path_.u8string() << " does not exist.";
    return false;
  }

  if (!std::filesystem::is_regular_file(file_status)) {
    *logger << "[WARNING] Path at "
        << path_.u8string()
        << " is not a regular file.";
    return false;
  }

  std::filesystem::perms file_permissions = file_status.permissions();
  if (((file_permissions & std::filesystem::perms::owner_read)
          | (file_permissions & std::filesystem::perms::group_read)
          | (file_permissions & std::filesystem::perms::others_read))
      != std::filesystem::perms::none) {
    *logger << "[WARNING] Insufficient read permissions for config file at "
        << path_.u8string();
    return false;
  }

  std::ifstream file(path_);
  if (!file.good()) {
    *logger << "[WARNING] Could not open config file at " << path_.u8string();
    return false;
  }

  // Clear pre-existing state.
  Clear();

  // Read each entry, line-by-line.
  for (int line_count = 1; file; ++line_count) {
    // Limit reads to prevent out-of-memory.
    std::array<char, kLineCapacity> line;
    line[line.size() - 2] = '\0';

    // If line exceeds character limit, skip the line.
    file.getline(line.data(), line.size(), '\n');
    if (line[line.size() - 2] != '\0') {
      *logger << "[WARNING] Config file "
          << path_.u8string()
          << " exceeded line length limit of "
          << (line.size() - 1)
          << " at line "
          << line_count;
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return false;
    }

    std::optional<Entry> entry_optional = ParseLine(line_count, line.data());
    if (!entry_optional.has_value()) {
      *logger << "[WARNING] Unable to parse config file "
          << path_.u8string()
          << " at line "
          << line_count;
      return false;
    }

    entry_optional.value();
  }

  return true;
}

bool Config::WriteFile() {
  return Config::WriteFile(path_);
}

bool Config::WriteFile(const std::filesystem::path& path) {
  return false;
}

void Config::Clear() {
}

std::optional<Entry> Config::ParseLine(int line_count, std::string_view line) {
  std::string_view unparsed = line;

  // Locate comment and remove anything that comes after.
  size_t comment_index = unparsed.find(Config::kComment);
  std::string comment;
  if (comment_index != std::string_view::npos) {
    comment = unparsed.substr(comment_index);
    unparsed = unparsed.substr(0, comment_index);
  }

  // Locate key-value delimiter.
  size_t key_value_delimiter_index = unparsed.find(
      Config::kKeyValueDelimiter);
  if (key_value_delimiter_index == std::string_view::npos) {
    return std::nullopt;
  }

  size_t value_index = key_value_delimiter_index + 1;

  return true;
}

} // namespace config
