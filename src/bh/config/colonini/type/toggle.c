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

#include "bh/config/colonini/type/toggle.h"

#include <windows.h>

/**
 * External
 */

struct Colonini_Toggle* Colonini_Toggle_Init(
    struct Colonini_Toggle* toggle, unsigned char enabled, BYTE key_code) {
  toggle->enabled = enabled;
  toggle->key_code = key_code;

  return toggle;
}

void Colonini_Toggle_Deinit(struct Colonini_Toggle* toggle) {
  toggle->key_code = 0;
  toggle->enabled = 0;
}
