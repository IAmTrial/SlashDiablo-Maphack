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

#ifndef BH_CONFIG_CONFIG_H_
#define BH_CONFIG_CONFIG_H_

#include <windows.h>

#include <filesystem>
#include <string>
#include <optional>
#include <variant>

// Usage of char and std::string in this file must be treated as UTF-8
// encoded strings.

namespace config {

struct KeyCode {
  DWORD code;
};

struct KeyCodeToggle {
  DWORD code;
  bool is_enabled;
};

using EntryValue = std::variant<
    bool, int, std::string, KeyCode, KeyCodeToggle>;

struct Entry {
  int line;
  std::string key;
  std::optional<std::string> subkey;
  std::variant<bool, int, std::string, KeyCode, KeyCodeToggle> value;
  std::string comment;
};

class Config {
 public:
  explicit Config(std::filesystem::path path) noexcept;

  bool ReadFile();

  bool WriteFile();

  bool WriteFile(const std::filesystem::path& path);

  constexpr const std::filesystem::path& path() const {
    return path_;
  }

 private:
  static constexpr std::string_view kComment = "//";
  static constexpr char kKeyValueDelimiter = ':';

  std::filesystem::path path_;

  /**
   * Primitive key-value access functions
   */

  /**
   * Gets the bool value that is mapped to the specified key.
   */
  std::optional<bool> GetBool(std::string_view key) const;

  /**
   * Gets the bool value that is mapped to the specified keys.
   */
  std::optional<bool> GetBool(
      std::string_view key, std::string_view subkey) const;

  /**
   * Gets the int value that is mapped to the specified key.
   */
  std::optional<int> GetInt(std::string_view key) const;

  /**
   * Gets the int value that is mapped to the specified keys.
   */
  std::optional<int> GetInt(
      std::string_view key, std::string_view subkey) const;

  /**
   * Gets the KeyCode that is mapped to the specified key.
   */
  std::optional<KeyCode> GetKeyCode(std::string_view key) const;

  /**
   * Gets the KeyCode that is mapped to the specified keys.
   */
  std::optional<KeyCode> GetKeyCode(
      std::string_view key, std::string_view subkey) const;

  /**
   * Gets the string value that is mapped to the specified key.
   */
  std::optional<std::string_view> GetString(std::string_view key) const;

  /**
   * Gets the string value that is mapped to the specified keys.
   */
  std::optional<std::string_view> GetString(
      std::string_view key, std::string_view subkey) const;

  /**
   * Container key-value access functions
   */

  /**
   * Gets the string value that is mapped to the specified keys.
   */
  std::span<std::string> GetArray(
      std::string_view key, std::string_view subkey) const;

  void Clear();
  std::optional<Entry> ParseLine(int line_count, std::string_view line);
  bool ParseKey(int line_count, std::string_view key_str);
  bool ParseValue(int line_count, std::string_view value_str);
};

} // namespace config

#endif /* BH_CONFIG_CONFIG_H_ */
