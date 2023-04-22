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

#include "bh/config/colonini/internal/parser/value_expr_type.h"

#include <assert.h>
#include <stddef.h>

/**
 * External
 */

const char* ValueExprType_GetDisplayName(
    enum ValueExprType type, size_t* length) {
  switch (type) {
    case ValueExprType_kEmpty: {
      static const char kDisplayName[] = "Empty";

      if (length != NULL) {
        *length = (sizeof(kDisplayName) / sizeof(kDisplayName[0])) - 1;
      }
      return kDisplayName;
    }

    case ValueExprType_kConst: {
      static const char kDisplayName[] = "Const";

      if (length != NULL) {
        *length = (sizeof(kDisplayName) / sizeof(kDisplayName[0])) - 1;
      }
      return kDisplayName;
    }

    case ValueExprType_kToggle: {
      static const char kDisplayName[] = "Toggle";

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
