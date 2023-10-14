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

#ifndef BH_CONFIG_COLONINI_TYPE_TOGGLE_H_
#define BH_CONFIG_COLONINI_TYPE_TOGGLE_H_

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct Colonini_Toggle {
  /** The enable state of the toggle. */
  unsigned char enabled;

  /**
   * The internal integer value of the key that switches the toggle
   * between enabled and disabled.
   */
  BYTE key_code;
};

struct Colonini_Toggle* Colonini_Toggle_Init(
    struct Colonini_Toggle* toggle, unsigned char enabled, BYTE key_code);

void Colonini_Toggle_Deinit(struct Colonini_Toggle* toggle);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_TYPE_TOGGLE_H_ */
