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

#include "bh/common/data_struct/red_black_tree/red_black_color.h"

#include <assert.h>
#include <stddef.h>

const char* RedBlackColor_GetDisplayName(
    enum RedBlackColor color, size_t* length) {
  switch (color) {
    case RedBlackColor_kRed: {
      static const char kDisplayName[] = "Red";

      if (length != NULL) {
        *length = (sizeof(kDisplayName) / sizeof(kDisplayName[0])) - 1;
      }
      return kDisplayName;
    }

    case RedBlackColor_kBlack: {
      static const char kDisplayName[] = "Black";

      if (length != NULL) {
        *length = (sizeof(kDisplayName) / sizeof(kDisplayName[0])) - 1;
      }
      return kDisplayName;
    }

    default: {
      assert(0 && "This should never happen.");
      return NULL;
    }
  }
}
