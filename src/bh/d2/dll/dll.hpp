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

#ifndef BH_D2_DLL_HPP_
#define BH_D2_DLL_HPP_

#include <string_view>

namespace bh::d2 {

enum class Dll {
  kUnspecified = 0,

  /*
   * Only dynamic-link libraries that bundle with Diablo II are
   * permitted to be added.
   *
   * The following libraries are products of other entities. Do not
   * add any of these files:
   * binkw32.dll, glide3x.dll, ijl11.dll, SmackW32.dll
   */

  kBnClient,
  kD2Client,
  kD2Cmp,
  kD2Common,
  kD2DDraw,
  kD2Direct3d,
  kD2Game,
  kD2Gdi,
  kD2Gfx,
  kD2Glide,
  kD2Lang,
  kD2Launch,
  kD2McpClient,
  kD2Multi,
  kD2Net,
  kD2Sound,
  kD2Win,
  kFog,
  kStorm,
};

inline constexpr auto kDlls = std::to_array<Dll>({
    Dll::kBnClient,
    Dll::kD2Client,
    Dll::kD2Cmp,
    Dll::kD2Common,
    Dll::kD2DDraw,
    Dll::kD2Direct3d,
    Dll::kD2Game,
    Dll::kD2Gdi,
    Dll::kD2Gfx,
    Dll::kD2Glide,
    Dll::kD2Lang,
    Dll::kD2Launch,
    Dll::kD2McpClient,
    Dll::kD2Multi,
    Dll::kD2Net,
    Dll::kD2Sound,
    Dll::kD2Win,
    Dll::kFog,
    Dll::kStorm,
});
static_assert(std::ranges::is_sorted(kDlls));

}  // namespace bh::d2

#endif  // BH_D2_DLL_HPP_
