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

/**
 * BH
 * Copyright (C) 2011  McGod
 *
 * All rights reserved.
 */

#ifndef BH_COMMON_INPUT_HPP_
#define BH_COMMON_INPUT_HPP_

#include <windows.h>

#include <optional>
#include <string_view>

namespace bh::common::input {

/**
 * A Virtual-Key with associated hexadecimal value and symbol constant
 * names.
 *
 * Information on Virtual-Key can be found at
 * https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
 */
struct VirtualKey {
  /**
   * The Virtual-Key hexadecimal value associated with the key.
   */
  BYTE code;

  /**
   * The Virtual-Key symbolic constant name, as a string
   * (e.g. VK_LBUTTON, VK_PRIOR, VK_OEM_4).
   */
  std::string_view symbol_name;

  /**
   * The Virtual-Key symbolic constant name, as a string
   * (e.g. VK_LBUTTON, VK_PAGEUP, VK_LEFTBRACKET). These are defined
   * for backward compatability with existing config key bindings.
   */
  std::string_view old_symbol_name;

  /**
   * The human-readable name for the key.
   */
  std::wstring_view common_name;

  /**
   * Gets a constant that represents an unset VirtualKey.
   */
  static inline constexpr VirtualKey GetUnset() {
    return VirtualKey { 0x00, "None", "None", L"Not Set" };
  }

  /**
   * Gets a Virtual-Key by its hexadecimal code value.
   */
  static const VirtualKey& GetFromCode(BYTE code);

  /**
   * Gets a Virtual-Key by its symbol constant name. If there is no
   * Virtual-Key with such a symbol name, then the function returns
   * nullopt.
   */
  static std::optional<VirtualKey> GetFromSymbolName(
      std::string_view symbol_name);
};

}  // namespace bh::common::input

#endif  // BH_COMMON_INPUT_HPP_
