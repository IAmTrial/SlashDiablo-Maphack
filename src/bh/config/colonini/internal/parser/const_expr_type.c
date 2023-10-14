
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

#include "bh/config/colonini/internal/parser/const_expr_type.h"

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include "bh/common/string_util/memstring.h"

static const char kDecimalDigits[] = "0123456789";
static const char kHexadecimalDigits[] = "0123456789ABCDEFabcdef";
static const char kFalse[] = "false";
static const char kTrue[] = "true";

enum {
  kDecimalDigitsLength =
      (sizeof(kDecimalDigits) / sizeof(kDecimalDigits[0])) - 1,
  kHexadecimalDigitsLength =
      (sizeof(kHexadecimalDigits) / sizeof(kHexadecimalDigits[0])) - 1,
  kFalseLength = (sizeof(kFalse) / sizeof(kFalse[0])) - 1,
  kTrueLength = (sizeof(kTrue) / sizeof(kTrue[0])) - 1
};

/**
 * External
 */

const char* ConstExprType_GetDisplayName(
    enum ConstExprType type, size_t* length) {
  switch (type) {
    case ConstExprType_kInvalid: {
      static const char kDisplayName[] = "Invalid";

      if (length != NULL) {
        *length = (sizeof(kDisplayName) / sizeof(kDisplayName[0])) - 1;
      }
      return kDisplayName;
    }

    case ConstExprType_kString: {
      static const char kDisplayName[] = "String";

      if (length != NULL) {
        *length = (sizeof(kDisplayName) / sizeof(kDisplayName[0])) - 1;
      }
      return kDisplayName;
    }

    case ConstExprType_kSignedInt: {
      static const char kDisplayName[] = "SignedInt";

      if (length != NULL) {
        *length = (sizeof(kDisplayName) / sizeof(kDisplayName[0])) - 1;
      }
      return kDisplayName;
    }

    case ConstExprType_kUnsignedInt: {
      static const char kDisplayName[] = "UnsignedInt";

      if (length != NULL) {
        *length = (sizeof(kDisplayName) / sizeof(kDisplayName[0])) - 1;
      }
      return kDisplayName;
    }

    case ConstExprType_kBoolean: {
      static const char kDisplayName[] = "Boolean";

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

enum ConstExprType ConstExprType_MatchString(
    const char* str, size_t length) {
  size_t i_invalid;
  enum ConstExprType type_candidate;

  if (length <= 0) {
    return ConstExprType_kInvalid;
  }

  /* Test true and false (boolean type). */
  if (length == kTrueLength || length == kFalseLength) {
    char temp[kFalseLength];
    size_t i;

    for (i = 0; i < length; ++i) {
      temp[i] = tolower(str[i]);
    }

    if (memcmp(temp, kFalse, kFalseLength) == 0
        || memcmp(temp, kTrue, kTrueLength) == 0) {
      return ConstExprType_kBoolean;
    }
  }

  /* Test decimal and hexadecimal (sint and uint types). */
  if (length >= 3 && str[0] == '0' && tolower(str[1]) == 'x') {
    type_candidate = ConstExprType_kUnsignedInt;
    i_invalid =
        MemCSpn(
            &str[2], length - 2, kHexadecimalDigits, kHexadecimalDigitsLength);
    /* Add 2 because if this is valid, then i_invalid would be length - 2. */
    i_invalid += 2;
  } else {
    type_candidate = ConstExprType_kSignedInt;
    i_invalid = MemCSpn(str, length, kDecimalDigits, kDecimalDigitsLength);
  }

  if (i_invalid == length) {
    return type_candidate;
  }

  return ConstExprType_kString;
}
