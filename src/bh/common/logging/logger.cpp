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

#include "bh/common/logging/logger.hpp"

#include <stddef.h>
#include <stdint.h>
#include <windows.h>

#include <algorithm>
#include <array>
#include <condition_variable>
#include <format>
#include <memory>
#include <mutex>
#include <ostream>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "bh/common/logging/level.hpp"
#include "bh/common/logging/log_record.hpp"
#include "bh/common/string_util/wide.hpp"

namespace bh::common::logging {
namespace {

using ::bh::common::string_util::wide::ToUtf8;

using LoggersType = std::unordered_map<std::wstring, Logger>;

#ifdef NDEBUG
static constexpr Level kDefaultLevel = Level::kWarn;
#else
static constexpr Level kDefaultLevel = Level::kDebug;
#endif  // NDEBUG

struct LoggerThreadSyncObject {
  std::mutex mutex;
  std::condition_variable cond_var;
};

static LoggerThreadSyncObject& GetLoggerThreadSyncObject() {
  static auto* sync_obj = new LoggerThreadSyncObject();
  return *sync_obj;
}

static LoggersType& GetLoggers() {
  static auto* loggers = new LoggersType();
  return *loggers;
}

}  // namespace

Logger::Logger(Level level, std::wstring src_path)
    : level_(std::move(level)),
      src_path_(std::move(src_path)),
      log_records_(),
      log_records_mutex_(std::make_unique<std::mutex>()),
      streams_() {}

Logger& Logger::GetLogger(std::wstring_view src_path) {
  const std::wstring key(src_path);

  LoggersType& loggers = GetLoggers();
  if (auto it = loggers.find(key); it != loggers.cend()) {
    return it->second;
  }

  Logger logger(kDefaultLevel, key);
  auto [it, _] = loggers.insert({ std::move(key), std::move(logger) });
  return it->second;
}

void Logger::VLog(
    uintmax_t src_line,
    Level level,
    std::string_view format,
    std::format_args args) {
  if (level < level_ || level == Level::kOff || level_ == Level::kOff) {
    return;
  }

#ifndef NDEBUG
  if (level == Level::kDebug) {
    return;
  }
#endif  // NDEBUG

  // Strip out any path prior to "src" or "BH", if possible.
  static constexpr auto kPathStarts = std::to_array<std::wstring_view>({
      L"/BH/",
      L"\\BH\\",
      L"/src/",
      L"\\src\\",
  });

  std::array<size_t, kPathStarts.size()> start_indices;
  std::ranges::transform(
      kPathStarts,
      start_indices.begin(),
      [this] (std::wstring_view str) {
        return this->src_path_.rfind(str);
      });
  size_t start_index = std::ranges::min(start_indices);

  std::string utf8_src_file_path =
      ToUtf8(
          (start_index == std::wstring_view::npos)
              ? src_path_
              : src_path_.substr(start_index));

  // Create formatted log record. Message should appear as:
  // SRC_FILE:SRC_LINE - USER_STR

  std::string message =
      std::format(
          "{0}:{1} - {2}",
          utf8_src_file_path,
          src_line,
          std::vformat(format, args));
  LogRecord log_record(level, std::move(message));

  // These braces limit the lock's scope.
  {
    std::lock_guard lock(*log_records_mutex_);
    log_records_.insert(std::move(log_record));
  }

  GetLoggerThreadSyncObject().cond_var.notify_all();
  StartOnceLoggerThread();
}

void Logger::VDebug(
    uintmax_t src_line,
    std::string_view format,
    std::format_args args) {
  VLog(src_line, Level::kDebug, format, args);
}

void Logger::VError(
    uintmax_t src_line,
    std::string_view format,
    std::format_args args) {
  VLog(src_line, Level::kError, format, args);
}

void Logger::VFatal(
    uintmax_t src_line,
    std::string_view format,
    std::format_args args) {
  VLog(src_line, Level::kFatal, format, args);
}

void Logger::VInfo(
    uintmax_t src_line,
    std::string_view format,
    std::format_args args) {
  VLog(src_line, Level::kInfo, format, args);
}

void Logger::VWarn(
    uintmax_t src_line,
    std::string_view format,
    std::format_args args) {
  VLog(src_line, Level::kWarn, format, args);
}

void Logger::AddStream(std::shared_ptr<std::ostream> stream) {
  streams_.push_back(std::move(stream));
}

void Logger::StartOnceLoggerThread() {
  static auto* thread =
      new std::thread(
          []() {
            auto& loggers = GetLoggers();
            auto& sync_obj = GetLoggerThreadSyncObject();

            while (true) {
              for (auto& [_, logger] : loggers) {
                std::lock_guard lock(*logger.log_records_mutex_);

                for (auto& stream_ptr : logger.streams_) {
                  std::ostream& stream = *stream_ptr;
                  for (const LogRecord& log_record : logger.log_records_) {
                    stream << log_record << '\n';
                  }
                  stream.flush();
                }
                logger.log_records_.clear();
              }
              std::unique_lock lock(sync_obj.mutex);
              sync_obj.cond_var.wait(lock);
            }
          });
}

}  // namespace bh::common::logging
