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

#include "bh/d2/d2win/function/v1_14d/load_mpq.hpp"

#include <stdint.h>

#include <variant>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/dll/address.hpp"
#include "bh/d2/dll/dll.hpp"
#include "bh/d2/exe/version.hpp"
#include "bh/d2/struct/v1_14d/mpq_handle.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::d2win::v1_14d {
namespace {

using ::bh::common::logging::Logger;
using ::bh::d2::dll::Dll;
using ::bh::d2::dll::GetAddress;
using ::bh::d2::dll::Offset;
using ::bh::d2::dll::Ordinal;
using ::bh::d2::exe::version::GetRunning;
using ::bh::d2::exe::version::Version;
using ::bh::d2::v1_14d::MpqHandle;
using ::bh::global::GetFileLogger;

using FuncType =
    MpqHandle* __fastcall(
        const char* mpq_path,
        int32_t is_set_err_on_drive_query_fail,
        void* (*on_fail_callback)(void),
        int32_t priority);

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

static std::variant<Offset, Ordinal> GetOffsetOrOrdinal(Version version) {
  switch (version) {
    case Version::k1_14d: {
      return Offset(0x117332);
    }
  }

  // This should never happen.
  GetLogger().Fatal(
      __LINE__,
      "Unhandled Version with value {:d}",
      static_cast<int>(version));
  return Offset(0);
}

}  // namespace

MpqHandle* LoadMpq(
    const char* mpq_path,
    int32_t is_set_err_on_drive_query_fail,
    void* (*on_fail_callback)(void),
    int32_t priority) {
  static FuncType* func =
      std::visit(
          [] (auto value) {
            void* address = GetAddress(Dll::kD2Win, value);
            return reinterpret_cast<FuncType*>(address);
          },
          GetOffsetOrOrdinal(GetRunning()));

  return func(
      mpq_path, is_set_err_on_drive_query_fail, on_fail_callback, priority);
}

}  // namespace bh::d2::d2win::v1_14d
