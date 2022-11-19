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

#ifndef BH_COMMON_LOGGING_LEVEL_HPP_
#define BH_COMMON_LOGGING_LEVEL_HPP_

#include <format>
#include <string_view>
#include <ostream>

namespace bh::common::logging {

enum class Level {
  kUnspecified = 0,

  kAll,
  kDebug,
  kInfo,
  kWarn,
  kError,
  kFatal,
  kOff,
};

}  // namespace bh::common::logging

template <>
struct std::formatter<::bh::common::logging::Level>
    : public std::formatter<std::string_view> {
  static constexpr std::string_view kDebugLogString = "DEBUG";
  static constexpr std::string_view kErrorLogString = "ERROR";
  static constexpr std::string_view kFatalLogString = "FATAL";
  static constexpr std::string_view kInfoLogString = "INFO";
  static constexpr std::string_view kWarnLogString = "WARN";

  template<class FormatContext>
  FormatContext::iterator format(
      ::bh::common::logging::Level level, FormatContext& context) const {
    using Level = ::bh::common::logging::Level;
    switch (level) {
      case Level::kDebug: {
        return std::formatter<std::string_view>::format(
            kDebugLogString, context);
      }

      case Level::kError: {
        return std::formatter<std::string_view>::format(
            kErrorLogString, context);
      }

      case Level::kFatal: {
        return std::formatter<std::string_view>::format(
            kFatalLogString, context);
      }

      case Level::kInfo: {
        return std::formatter<std::string_view>::format(
            kInfoLogString, context);
      }

      case Level::kWarn: {
        return std::formatter<std::string_view>::format(
            kWarnLogString, context);
      }
    }

    return context.out();
  }
};

#endif  // BH_COMMON_LOGGING_LEVEL_HPP_
