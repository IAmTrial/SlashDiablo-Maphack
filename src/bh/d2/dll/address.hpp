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

#ifndef BH_D2_DLL_ADDRESS_HPP_
#define BH_D2_DLL_ADDRESS_HPP_

#include <stddef.h>

#include "bh/d2/dll/dll.hpp"

namespace bh::d2::dll {

struct Offset {
  explicit constexpr Offset(ptrdiff_t offset) noexcept : offset(offset) {}

  constexpr operator ptrdiff_t() const noexcept {
    return offset;
  }

  ptrdiff_t offset;
};

struct Ordinal {
  explicit constexpr Ordinal(unsigned short ordinal) noexcept
      : ordinal(ordinal) {}

  constexpr operator unsigned short() const noexcept {
    return ordinal;
  }

  explicit operator const char*() const noexcept {
    return reinterpret_cast<const char*>(ordinal);
  }

  unsigned short ordinal;
};

/**
 * Get a DLL address with an offset.
 */
void* GetAddress(Dll dll, Offset offset);

/**
 * Get a DLL address with an ordinal.
 */
void* GetAddress(Dll dll, Ordinal ordinal);

}  // namespace bh::d2::dll

#endif  // BH_D2_DLL_ADDRESS_HPP_
