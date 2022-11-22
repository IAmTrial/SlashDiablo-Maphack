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

#include "bh/d2/storm/function/universal/file_close_archive.hpp"

#include <assert.h>
#include <windows.h>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/exe/version.hpp"
#include "bh/d2/storm/function/v1_00/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_07/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_08/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_09/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_09b/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_09d/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_10/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_11/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_11b/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_12/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_13c/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_13d/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_14/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_14b/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_14c/file_close_archive.hpp"
#include "bh/d2/storm/function/v1_14d/file_close_archive.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::storm::universal {
namespace {

using ::bh::common::logging::Logger;
using ::bh::d2::exe::version::GetRunning;
using ::bh::d2::exe::version::Version;
using ::bh::global::GetFileLogger;

namespace d2 = ::bh::d2;
namespace storm = ::bh::d2::storm;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

}  // namespace

BOOL SFileCloseArchive(HANDLE mpq) {
  Version version = GetRunning();
  switch (version) {
    case Version::k1_00: {
      return storm::v1_00::SFileCloseArchive(mpq);
    }

    case Version::k1_07: {
      return storm::v1_07::SFileCloseArchive(mpq);
    }

    case Version::k1_08: {
      return storm::v1_08::SFileCloseArchive(mpq);
    }

    case Version::k1_09: {
      return storm::v1_09::SFileCloseArchive(mpq);
    }

    case Version::k1_09b: {
      return storm::v1_09b::SFileCloseArchive(mpq);
    }

    case Version::k1_09d: {
      return storm::v1_09d::SFileCloseArchive(mpq);
    }

    case Version::k1_10: {
      return storm::v1_10::SFileCloseArchive(mpq);
    }

    case Version::k1_11: {
      return storm::v1_11::SFileCloseArchive(mpq);
    }

    case Version::k1_11b: {
      return storm::v1_11b::SFileCloseArchive(mpq);
    }

    case Version::k1_12: {
      return storm::v1_12::SFileCloseArchive(mpq);
    }

    case Version::k1_13c: {
      return storm::v1_13c::SFileCloseArchive(mpq);
    }

    case Version::k1_13d: {
      return storm::v1_13d::SFileCloseArchive(mpq);
    }

    case Version::k1_14: {
      return storm::v1_14::SFileCloseArchive(mpq);
    }

    case Version::k1_14b: {
      return storm::v1_14b::SFileCloseArchive(mpq);
    }

    case Version::k1_14c: {
      return storm::v1_14c::SFileCloseArchive(mpq);
    }

    case Version::k1_14d: {
      return storm::v1_14d::SFileCloseArchive(mpq);
    }
  }

  // This should never happen.
  GetLogger().Fatal(
      __LINE__,
      "Unhandled Version with value {:d}",
      static_cast<int>(version));
  return FALSE;
}

}  // namespace bh::d2::storm::universal
