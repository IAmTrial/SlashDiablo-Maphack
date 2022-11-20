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

#ifndef BH_D2_STRUCT_V1_00_MPQ_HANDLE_HPP_
#define BH_D2_STRUCT_V1_00_MPQ_HANDLE_HPP_

#include <stddef.h>
#include <windows.h>

#include <type_traits>

namespace bh::d2::v1_00 {

/* sizeof: 264 */ struct MpqHandle {
  /* 0 */ HANDLE handle;
  /* 4 */ char path[/* 260 */ MAX_PATH];
};

static_assert(std::is_standard_layout_v<MpqHandle>);
static_assert(std::is_trivial_v<MpqHandle>);
static_assert(sizeof(MpqHandle) >= 264);
static_assert(offsetof(MpqHandle, handle) == 0);
static_assert(offsetof(MpqHandle, path) == 4);

}  // namespace bh::d2::v1_00

#endif  // BH_D2_STRUCT_V1_00_MPQ_HANDLE_HPP_
