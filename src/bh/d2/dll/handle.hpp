/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2022  SlashDiablo Community
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

#ifndef BH_D2_DLL_HANDLE_HPP_
#define BH_D2_DLL_HANDLE_HPP_

#include <windows.h>

#include "bh/d2/dll/dll.hpp"

namespace bh::d2::dll {

HMODULE GetHandle(Dll dll);

}  // namespace bh::d2::dll

#endif  // BH_D2_DLL_HANDLE_HPP_
