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

#include "bh/d2/storm/function/file_close_archive.hpp"

#include <assert.h>
#include <windows.h>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/exe/version.hpp"
#include "bh/d2/storm/function/v1_00/file_close_archive.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::storm {

using ::bh::common::logging::Logger;
using ::bh::d2::exe::version::GetRunning;
using ::bh::d2::exe::version::Version;
using ::bh::global::GetFileLogger;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

BOOL SFileCloseArchive(HANDLE mpq) {
  Version version = GetRunning();
  switch (version) {
    case Version::k1_13c:
    case Version::k1_13d:
    case Version::k1_14d: {
      return v1_00::SFileCloseArchive(mpq);
    }
  }

  // This should never happen.
  GetLogger().Fatal(
      __LINE__,
      "Unhandled Version with value {:d}",
      static_cast<int>(version));
  return FALSE;
}

}  // namespace bh::d2::storm
