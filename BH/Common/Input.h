/**
 * SlashDiablo Maphack
 * Copyright (C) SlashDiablo Community
 *
 * BH
 * Copyright 2011 (C) McGod
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

#ifndef BH_COMMON_INPUT_H_
#define BH_COMMON_INPUT_H_

#include <windows.h>

#include <optional>
#include <string_view>

namespace common::input {

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
  std::string_view common_name;

  static inline constexpr VirtualKey GetUnset() {
    return VirtualKey { 0x00, "None", "None", "Not Set" };
  }

  static const VirtualKey& GetFromCode(BYTE code);

  static std::optional<VirtualKey> GetFromSymbolName(
      std::string_view symbol_name);
};

} // common::input

#endif // BH_COMMON_INPUT_H_
