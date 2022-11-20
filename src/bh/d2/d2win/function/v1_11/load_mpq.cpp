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

#include "bh/d2/d2win/function/v1_11/load_mpq.hpp"

#include <assert.h>
#include <stdint.h>

#include <variant>

#include "bh/d2/dll/address.hpp"
#include "bh/d2/dll/dll.hpp"
#include "bh/d2/exe/version.hpp"
#include "bh/d2/struct/v1_00/mpq_handle.hpp"

namespace bh::d2::d2win::v1_11 {
namespace {

using ::bh::d2::dll::Dll;
using ::bh::d2::dll::GetAddress;
using ::bh::d2::dll::Offset;
using ::bh::d2::dll::Ordinal;
using ::bh::d2::exe::version::GetRunning;
using ::bh::d2::exe::version::Version;
using ::bh::d2::v1_00::MpqHandle;

using FuncType =
    MpqHandle* __cdecl(
        const char*, const char*, const char*, int32_t, void*, int32_t);

static std::variant<Offset, Ordinal> GetOffsetOrOrdinal(Version version) {
  switch (version) {
    case Version::k1_13c: {
      return Offset(0x7E60);
    }

    case Version::k1_13d: {
      return Offset(0x7E50);
    }
  }

  // This should never happen.
  assert(false);
  return Offset(0);
}

static __declspec(naked) MpqHandle* __cdecl CallShim(
    FuncType* func,
    const char* dll_filename,
    const char* mpq_path,
    const char* mpq_name,
    int32_t is_set_err_on_drive_query_fail,
    void* (*on_fail_callback),
    int32_t priority) {
  __asm {
    push ebp
    mov ebp, esp

    push ecx
    push edx

    mov eax, dword ptr [ebp + 32]
    push dword ptr [ebp + 28]
    push dword ptr [ebp + 24]
    push dword ptr [ebp + 20]
    push dword ptr [ebp + 16]
    push dword ptr [ebp + 12]
    call dword ptr [ebp + 8]

    pop edx
    pop ecx

    leave
    ret
  }
}

}  // namespace

MpqHandle* LoadMpq(
    const char* dll_filename,
    const char* mpq_path,
    const char* mpq_name,
    int32_t is_set_err_on_drive_query_fail,
    void* (*on_fail_callback),
    int32_t priority) {
  static FuncType* func =
      std::visit(
          [] (auto value) {
            void* address = GetAddress(Dll::kD2Win, value);
            return reinterpret_cast<FuncType*>(address);
          },
          GetOffsetOrOrdinal(GetRunning()));

  return CallShim(
      func,
      dll_filename,
      mpq_path,
      mpq_name,
      is_set_err_on_drive_query_fail,
      on_fail_callback,
      priority);
}

}  // namespace bh::d2::d2lang::v1_11
