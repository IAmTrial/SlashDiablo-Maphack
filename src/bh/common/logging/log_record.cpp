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

#include "bh/common/logging/log_record.hpp"

#include <chrono>
#include <format>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

#include "bh/common/logging/level.hpp"

namespace bh::common::logging {

std::ostream& operator<<(std::ostream& stream, const LogRecord& log_record) {
  // Format should appear as:
  // TIMESTAMP  [LEVEL] MESSAGE
  std::string str =
      std::format(
          "{0:%F} {0:%T}  [Thread.{1}] [{2}] {3}",
          log_record.create_time_,
          (std::ostringstream() << log_record.thread_id_).str(),
          log_record.level_,
          log_record.message_);
  return stream << str;
}

}  // namespace bh::common::logging
