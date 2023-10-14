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

#include "bh/config/colonini/type/data_type.h"

#include <assert.h>
#include <stddef.h>

const char* Colonini_DataType_GetDisplayName(
    enum Colonini_DataType type, size_t* length) {
  switch (type) {
    case Colonini_DataType_kBoolean: {
      static const char kDisplayName[] = "Boolean";

      if (length != NULL) {
        *length = sizeof(kDisplayName) / sizeof(kDisplayName[0]) - 1;
      }
      return kDisplayName;
    }

    case Colonini_DataType_kInteger: {
      static const char kDisplayName[] = "Integer";

      if (length != NULL) {
        *length = sizeof(kDisplayName) / sizeof(kDisplayName[0]) - 1;
      }
      return kDisplayName;
    }

    case Colonini_DataType_kString: {
      static const char kDisplayName[] = "String";

      if (length != NULL) {
        *length = sizeof(kDisplayName) / sizeof(kDisplayName[0]) - 1;
      }
      return kDisplayName;
    }

    case Colonini_DataType_kToggle: {
      static const char kDisplayName[] = "Toggle";

      if (length != NULL) {
        *length = sizeof(kDisplayName) / sizeof(kDisplayName[0]) - 1;
      }
      return kDisplayName;
    }

    case Colonini_DataType_kUnspecified:
    default: {
      assert(0 && "This should never happen.");
      return NULL;
    }
  }
}
