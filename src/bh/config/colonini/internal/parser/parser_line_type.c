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

#include "bh/config/colonini/internal/parser/parser_line_type.h"

#include <assert.h>
#include <stddef.h>

const char* ParserLineType_GetDisplayName(
    enum ParserLineType type, size_t* length) {
  switch (type) {
    case ParserLineType_kInvalid: {
      static const char kDisplayName[] = "Invalid";

      if (length != NULL) {
        *length = (sizeof(kDisplayName) / sizeof(kDisplayName[0])) - 1;
      }
      return kDisplayName;
    }

    case ParserLineType_kNoOp: {
      static const char kDisplayName[] = "No Op";

      if (length != NULL) {
        *length = (sizeof(kDisplayName) / sizeof(kDisplayName[0])) - 1;
      }
      return kDisplayName;
    }

    case ParserLineType_kAssignStatement: {
      static const char kDisplayName[] = "Assign Statement";

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

