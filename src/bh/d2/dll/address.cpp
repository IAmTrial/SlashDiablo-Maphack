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

#include "bh/d2/dll/address.hpp"

#include <stddef.h>
#include <stdint.h>
#include <windows.h>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/dll/dll.hpp"
#include "bh/d2/dll/handle.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::dll {
namespace {

using ::bh::common::logging::Logger;
using ::bh::global::GetFileLogger;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

}  // namespace

void* GetAddress(Dll dll, Offset offset) {
  HMODULE module_handle = GetHandle(dll);
  unsigned char* address = reinterpret_cast<unsigned char*>(module_handle);
  return address + offset;
}

void* GetAddress(Dll dll, Ordinal ordinal) {
  HMODULE module_handle = GetHandle(dll);
  FARPROC proc_address =
      GetProcAddress(module_handle, static_cast<LPCSTR>(ordinal));
  if (proc_address == nullptr) {
    GetLogger().Fatal(
        __LINE__,
        "GetProcAddress(0x{:X}, {}) failed with error code 0x{:X}.",
        reinterpret_cast<uintptr_t>(module_handle),
        static_cast<unsigned short>(ordinal),
        GetLastError());
    return nullptr;
  }

  return reinterpret_cast<void*>(proc_address);
}

}  // namespace bh::d2::dll
