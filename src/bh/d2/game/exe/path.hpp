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

#ifndef BH_D2_GAME_EXE_PATH_HPP_
#define BH_D2_GAME_EXE_PATH_HPP_

#include <string_view>

namespace bh::d2::game::exe {

/**
 * Returns a null-terminated string that represents the absolute path
 * to the current running executable
 * (e.g. "C:\Program Files\Diablo II\Game.exe").
 */
std::wstring_view GetAbsolutePath();

}  // namespace bh::d2::game::exe

#endif  // BH_D2_GAME_EXE_PATH_HPP_
