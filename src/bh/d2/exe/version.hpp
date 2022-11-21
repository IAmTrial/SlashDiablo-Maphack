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

#ifndef BH_D2_EXE_VERSION_HPP_
#define BH_D2_EXE_VERSION_HPP_

#include <array>
#include <string_view>

namespace bh::d2::exe::version {

enum class Version {
  kUnspecified = 0,

  // This is just used as a reference point.
  k1_00,

  k1_07,
  k1_08,
  k1_09,
  k1_09b,
  k1_09d,
  k1_10,
  k1_11,
  k1_11b,
  k1_12,
  k1_13c,
  k1_13d,
  k1_14,
  k1_14b,
  k1_14c,
  k1_14d,
};

inline constexpr auto kLodVersions = std::to_array<Version>({
    Version::k1_07,
    Version::k1_08,
    Version::k1_09,
    Version::k1_09b,
    Version::k1_09d,
    Version::k1_10,
    Version::k1_11,
    Version::k1_11b,
    Version::k1_12,
    Version::k1_13c,
    Version::k1_13d,
    Version::k1_14,
    Version::k1_14b,
    Version::k1_14c,
    Version::k1_14d,
});
static_assert(std::ranges::is_sorted(kLodVersions));

/**
 * Get the currently running version of Diablo II.
 */
Version GetRunning();

/**
 * Returns a null-terminated string that holds the display name for
 * the specified verison.
 */
std::wstring_view GetDisplayName(Version version);

}  // namespace bh::d2::exe::version

#endif  // BH_D2_EXE_VERSION_HPP_
