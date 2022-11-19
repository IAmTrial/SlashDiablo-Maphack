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

#ifndef BH_COMMON_LOGGING_LOG_RECORD_HPP_
#define BH_COMMON_LOGGING_LOG_RECORD_HPP_

#include <chrono>
#include <compare>
#include <ostream>
#include <string>
#include <thread>
#include <utility>

#include "bh/common/logging/level.hpp"

namespace bh::common::logging {

class LogRecord {
 public:
  using Clock = std::chrono::system_clock;

  /**
   * Creates an instance of a LogRecord with the specified level and
   * message.
   */
  LogRecord(
      Level level,
      std::string message)
          : level_(std::move(level)),
            message_(std::move(message)),
            create_time_(Clock::now()),
            thread_id_(std::this_thread::get_id()) {}

  friend std::ostream& operator<<(
      std::ostream& stream, const LogRecord& log_record);

  friend constexpr std::strong_ordering operator<=>(
      const LogRecord& first, const LogRecord& second) {
    auto make_cmp_tie =
        [] (const LogRecord& log_record) {
          return std::tie(
              log_record.create_time_,
              // Two records from the same thread are very unlikely to
              // have the same timestamp.
              log_record.thread_id_,
              log_record.level_,
              log_record.message_);
        };

    return make_cmp_tie(first) <=> make_cmp_tie(second);
  }

 private:
  /** Logging level of this record. */
  Level level_;

  /** UTF-8 null-terminated string containing the log message. */
  std::string message_;

  /** Time point at which this log record was created. */
  Clock::time_point create_time_;

  /** Thread ID of the creating thread. */
  std::thread::id thread_id_;
};

}  // namespace bh::common::logging

#endif  // BH_COMMON_LOGGING_LOG_RECORD_HPP_
