/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2023  SlashDiablo Team
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

#ifndef BH_COMMON_INPUT_VIRTUAL_KEY_H_
#define BH_COMMON_INPUT_VIRTUAL_KEY_H_

#include <stddef.h>
#include <wchar.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * A Virtual-Key with associated integral value and symbol constant
 * names.
 *
 * Information on Virtual-Key can be found at
 * https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
 */
struct Input_VirtualKey {
  /** The Virtual-Key integral value associated with the key. */
  BYTE code;

  /**
   * The Virtual-Key symbolic constant name, as a string
   * (e.g. VK_LBUTTON, VK_PRIOR, VK_OEM_4).
   */
  const char* symbol_name;

  /** The length of the symbol name. */
  size_t symbol_name_length;

  /**
   * The Virtual-Key symbolic constant name, as a string
   * (e.g. VK_LBUTTON, VK_PAGEUP, VK_LEFTBRACKET). These are defined
   * for backward compatability with existing config key bindings.
   */
  const char* old_symbol_name;

  /** The length of the old symbol name. */
  size_t old_symbol_name_length;

  /** The human-readable name for the key. */
  const wchar_t* common_name;

  /** The length of the common name. */
  size_t common_name_length;
};

/**
 * A constant that represents an unset VirtualKey.
 */
extern const struct Input_VirtualKey* Input_VirtualKey_kUnset;

/**
 * Compares the key codes of the left and right VirtualKey. Returns
 * a negative, zero, or positive compare value.
 */
int Input_VirtualKey_CompareCode(
    const struct Input_VirtualKey* left, const struct Input_VirtualKey* right);

/**
 * Gets a Virtual-Key by its integral code value.
 */
const struct Input_VirtualKey* Input_VirtualKey_GetFromCode(BYTE code);

/**
 * Gets a Virtual-Key by its symbol constant name. Lookup is case insensitive.
 * If there is no Virtual-Key with such a symbol name, then the function
 * returns NULL.
 */
const struct Input_VirtualKey* Input_VirtualKey_GetFromSymbolName(
    const char* symbol_name, size_t symbol_name_length);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_INPUT_VIRTUAL_KEY_H_ */
