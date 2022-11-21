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

#include "bh/d2/d2win/function/universal/load_mpq.hpp"

#include <assert.h>
#include <stdint.h>
#include <wchar.h>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/d2win/function/v1_13c/load_mpq.hpp"
#include "bh/d2/d2win/function/v1_13d/load_mpq.hpp"
#include "bh/d2/d2win/function/v1_14d/load_mpq.hpp"
#include "bh/d2/exe/version.hpp"
#include "bh/d2/struct/universal/mpq_handle.hpp"
#include "bh/d2/struct/v1_13c/mpq_handle.hpp"
#include "bh/d2/struct/v1_13d/mpq_handle.hpp"
#include "bh/d2/struct/v1_14d/mpq_handle.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::d2win::universal {
namespace {

using ::bh::common::logging::Logger;
using ::bh::d2::exe::version::GetRunning;
using ::bh::d2::exe::version::Version;
using ::bh::global::GetFileLogger;

namespace d2 = ::bh::d2;
namespace d2win = ::bh::d2::d2win;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

}  // namespace

d2::universal::MpqHandle* LoadMpq(
    const char* dll_filename,
    const char* mpq_path,
    const char* mpq_name,
    bool is_set_err_on_drive_query_fail,
    void* (*on_fail_callback)(void),
    int priority) {
  Version version = GetRunning();
  switch (version) {
    case Version::k1_13c: {
      d2::v1_13c::MpqHandle* handle_v1_13c =
          d2win::v1_13c::LoadMpq(
              dll_filename,
              mpq_path,
              mpq_name,
              is_set_err_on_drive_query_fail,
              on_fail_callback,
              priority);
      return reinterpret_cast<d2::universal::MpqHandle*>(handle_v1_13c);
    }

    case Version::k1_13d: {
      d2::v1_13d::MpqHandle* handle_v1_13d =
          d2win::v1_13d::LoadMpq(
              dll_filename,
              mpq_path,
              mpq_name,
              is_set_err_on_drive_query_fail,
              on_fail_callback,
              priority);
      return reinterpret_cast<d2::universal::MpqHandle*>(handle_v1_13d);
    }

    case Version::k1_14d: {
      d2::v1_14d::MpqHandle* handle_v1_14d =
          d2win::v1_14d::LoadMpq(
              mpq_path,
              is_set_err_on_drive_query_fail,
              on_fail_callback,
              priority);
      return reinterpret_cast<d2::universal::MpqHandle*>(handle_v1_14d);
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
