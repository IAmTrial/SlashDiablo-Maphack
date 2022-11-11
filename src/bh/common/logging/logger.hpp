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

#ifndef BH_COMMON_LOGGING_LOGGER_HPP_
#define BH_COMMON_LOGGING_LOGGER_HPP_

#include <stdint.h>
#include <windows.h>

#include <array>
#include <compare>
#include <condition_variable>
#include <format>
#include <memory>
#include <mutex>
#include <ostream>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include <utility>

#include "bh/common/logging/level.hpp"
#include "bh/common/logging/log_record.hpp"

namespace bh::common::logging {

class Logger {
 public:
  /**
   * Gets the global logger associated with the specified file.
   */
  static Logger& GetLogger(std::wstring_view src_path);

  /**
   * Logs formatted output at the specified level, using the same format
   * string specifications as std::format.
   */
  template <class ...Args>
  inline void Log(
      uintmax_t src_line,
      Level level,
      std::string_view format,
      Args&&... args) {
    VLog(src_line, level, format, std::make_format_args(args...));
  }

  /**
   * Logs formatted output at debug level, using the same format string
   * specifications as std::format.
   *
   * This function does nothing is NDEBUG is defined.
   */
  template <class ...Args>
  inline void Debug(
      uintmax_t src_line,
      std::string_view format,
      Args&&... args) {
    VDebug(src_line, format, std::make_format_args(args...));
  }

  /**
   * Logs formatted output at error level, using the same format string
   * specifications as std::format.
   */
  template <class ...Args>
  inline void Error(
      uintmax_t src_line,
      std::string_view format,
      Args&&... args) {
    VError(src_line, format, std::make_format_args(args...));
  }

  /**
   * Logs formatted output at fatal level, using the same format string
   * specifications as std::format.
   */
  template <class ...Args>
  inline void Fatal(
      uintmax_t src_line,
      std::string_view format,
      Args&&... args) {
    VFatal(src_line, format, std::make_format_args(args...));
  }

  /**
   * Logs formatted output at info level, using the same format string
   * specifications as std::format.
   */
  template <class ...Args>
  inline void Info(
      uintmax_t src_line,
      std::string_view format,
      Args&&... args) {
    VInfo(src_line, format, std::make_format_args(args...));
  }

  /**
   * Logs formatted output at warn level, using the same format string
   * specifications as std::format.
   */
  template <class ...Args>
  inline void Warn(
      uintmax_t src_line,
      std::string_view format,
      Args&&... args) {
    VWarn(src_line, format, std::make_format_args(args...));
  }

  /**
   * Logs formatted output at the specified level, using the same format
   * string specifications as std::vformat.
   */
  void VLog(
      uintmax_t src_line,
      Level level,
      std::string_view format,
      std::format_args args);

  /**
   * Logs formatted output at debug level, using the same format string
   * specifications as std::vformat.
   *
   * This function does nothing if NDEBUG is defined.
   */
  void VDebug(
      uintmax_t src_line,
      std::string_view format,
      std::format_args args);

  /**
   * Logs formatted output at error level, using the same format string
   * specifications as std::vformat.
   */
  void VError(
      uintmax_t src_line,
      std::string_view format,
      std::format_args args);

  /**
   * Logs formatted output at fatal level, using the same format string
   * specifications as std::vformat.
   */
  void VFatal(
      uintmax_t src_line,
      std::string_view format,
      std::format_args args);

  /**
   * Logs formatted output at info level, using the same format string
   * specifications as std::vformat.
   */
  void VInfo(
      uintmax_t src_line,
      std::string_view format,
      std::format_args args);

  /**
   * Logs formatted output at warn level, using the same format string
   * specifications as std::vformat.
   */
  void VWarn(
      uintmax_t src_line,
      std::string_view format,
      std::format_args args);

  /**
   * Adds an output stream that must only be used or shared by other
   * loggers.
   */
  void AddStream(std::shared_ptr<std::ostream> stream);

  constexpr Level level() const noexcept {
    return level_;
  }

  constexpr void set_level(Level level) noexcept {
    level_ = level;
  }

  constexpr std::wstring_view src_path() const noexcept {
    return src_path_;
  }

 private:
  using StreamsType = std::vector<std::shared_ptr<std::ostream>>;

  Logger(Level level, std::wstring src_path);

  /**
   * Start a thread dedicated to logging. This avoids the slowdown of
   * other threads due to I/O.
   */
  static void StartOnceLoggerThread();

  Level level_;
  std::wstring src_path_;
  std::multiset<LogRecord> log_records_;

  // Mutex does not have a move contructor, so this is a bypass to
  // allow move semantics.
  std::unique_ptr<std::mutex> log_records_mutex_;
  StreamsType streams_;
};

}  // namespace bh::common::logging

#endif  // BH_COMMON_LOGGING_LOGGER_HPP_
