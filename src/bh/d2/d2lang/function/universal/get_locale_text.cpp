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

#include "bh/d2/d2lang/function/universal/get_locale_text.hpp"

#include <wchar.h>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/d2lang/function/v1_13c/get_locale_text.hpp"
#include "bh/d2/d2lang/function/v1_13d/get_locale_text.hpp"
#include "bh/d2/d2lang/function/v1_14d/get_locale_text.hpp"
#include "bh/d2/exe/version.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::d2lang::universal {
namespace {

using ::bh::common::logging::Logger;
using ::bh::d2::exe::version::GetRunning;
using ::bh::d2::exe::version::Version;
using ::bh::global::GetFileLogger;

namespace v1_13c = ::bh::d2::d2lang::v1_13c;
namespace v1_13d = ::bh::d2::d2lang::v1_13d;
namespace v1_14d = ::bh::d2::d2lang::v1_14d;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

}  // namespace

const wchar_t* GetLocaleText(unsigned short id) {
  Version version = GetRunning();
  switch (GetRunning()) {
    case Version::k1_13c: {
      return v1_13c::GetLocaleText(id);
    }

    case Version::k1_13d: {
      return v1_13d::GetLocaleText(id);
    }

    case Version::k1_14d: {
      return v1_14d::GetLocaleText(id);
    }
  }

  // This should never happen.
  GetLogger().Fatal(
      __LINE__,
      "Unhandled Version with value {:d}",
      static_cast<int>(version));
  return nullptr;
}

}  // namespace bh::d2::d2lang::universal
