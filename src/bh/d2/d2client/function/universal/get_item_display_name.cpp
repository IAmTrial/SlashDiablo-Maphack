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

/**
 * BH
 * Copyright (C) 2011  McGod
 *
 * All rights reserved.
 */

#include "bh/d2/d2client/function/universal/get_item_display_name.hpp"

#include <wchar.h>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/d2client/function/v1_00/get_item_display_name.hpp"
#include "bh/d2/d2client/function/v1_10/get_item_display_name.hpp"
#include "bh/d2/d2client/function/v1_13c/get_item_display_name.hpp"
#include "bh/d2/d2client/function/v1_13d/get_item_display_name.hpp"
#include "bh/d2/struct/universal/unit.hpp"
#include "bh/d2/struct/v1_00/unit.hpp"
#include "bh/d2/struct/v1_10/unit.hpp"
#include "bh/d2/struct/v1_13c/unit.hpp"
#include "bh/d2/struct/v1_13d/unit.hpp"
#include "bh/d2/exe/version.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::d2client::universal {
namespace {

using ::bh::common::logging::Logger;
using ::bh::d2::exe::version::GetRunning;
using ::bh::d2::exe::version::Version;
using ::bh::global::GetFileLogger;

namespace d2 = ::bh::d2;
namespace d2client = ::bh::d2::d2client;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

}  // namespace

void GetItemDisplayName(
    const ::bh::d2::universal::Unit* unit,
    wchar_t* display_name,
    int display_name_max_length) {
  Version version = GetRunning();
  switch (GetRunning()) {
    case Version::k1_00: {
      d2client::v1_00::GetItemDisplayName(
          reinterpret_cast<const d2::v1_00::Unit*>(unit),
          display_name,
          display_name_max_length);
    }

    case Version::k1_10: {
      d2client::v1_10::GetItemDisplayName(
          reinterpret_cast<const d2::v1_10::Unit*>(unit),
          display_name,
          display_name_max_length);
    }

    case Version::k1_13c: {
      d2client::v1_13c::GetItemDisplayName(
          reinterpret_cast<const d2::v1_13c::Unit*>(unit),
          display_name,
          display_name_max_length);
    }

    case Version::k1_13d: {
      d2client::v1_13d::GetItemDisplayName(
          reinterpret_cast<const d2::v1_13d::Unit*>(unit),
          display_name,
          display_name_max_length);
    }
  }

  // This should never happen.
  GetLogger().Fatal(
      __LINE__,
      "Unhandled Version with value {:d}",
      static_cast<int>(version));
}

}  // namespace bh::d2::d2client::universal
