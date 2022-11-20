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

#include "bh/d2/d2win/function/load_mpq.hpp"

#include <assert.h>
#include <stdint.h>
#include <wchar.h>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/d2win/function/v1_11/load_mpq.hpp"
#include "bh/d2/exe/version.hpp"
#include "bh/d2/struct/mpq_handle.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::d2win {

using ::bh::d2::exe::version::GetRunning;
using ::bh::d2::exe::version::Version;
using ::bh::common::logging::Logger;
using ::bh::d2::exe::version::GetRunning;
using ::bh::d2::exe::version::Version;
using ::bh::d2::MpqHandle;
using ::bh::global::GetFileLogger;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

MpqHandle* LoadMpq(
    const char* dll_filename,
    const char* mpq_path,
    const char* mpq_name,
    bool is_set_err_on_drive_query_fail,
    void* (*on_fail_callback),
    int priority) {
  Version version = GetRunning();
  switch (version) {
    case Version::k1_13c:
    case Version::k1_13d: {
      v1_00::MpqHandle* handle =
          v1_11::LoadMpq(
              dll_filename,
              mpq_path,
              mpq_name,
              is_set_err_on_drive_query_fail,
              on_fail_callback,
              priority);
      return reinterpret_cast<MpqHandle*>(handle);
    }
  }

  // This should never happen.
  GetLogger().Fatal(
      __LINE__,
      "Unhandled Version with value {:d}",
      static_cast<int>(version));
  return FALSE;
}

}  // namespace bh::d2::d2win
