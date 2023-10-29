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

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "bh/common/string_util/integer.h"

static const char kDigitChars[] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e',
  'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
  'u', 'v', 'w', 'x', 'y', 'z'
};

enum {
  kDigitCharCount = sizeof(kDigitChars) / sizeof(kDigitChars[0])
};

static void BeforeAllSetUp(void) {}

struct EachContext {
  unsigned char unused;
};

typedef void TestFunc(struct EachContext* context);

static void BeforeEach(struct EachContext* context) {}

static void AfterEach(struct EachContext* context) {}

static void FromDigitChar_DigitChars_Converted(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kDigitCharCount; ++i) {
    int* value_convert_result;
    int value;

    value_convert_result =
        Integer_FromDigitChar(&value, kDigitChars[i], kDigitCharCount);

    assert(value_convert_result != NULL);
    assert(value == i);
  }
}

static void FromDigitChar_InvalidBase_ReturnsNull(
    struct EachContext* context) {
  int value;
  int* value_convert_result;

  value_convert_result = Integer_FromDigitChar(&value, '0', 9999);

  assert(value_convert_result == NULL);
}

static void FromDigitChar_OutOfBaseRange_ReturnsNull(
    struct EachContext* context) {
  int value;
  int* value_convert_result;

  value_convert_result = Integer_FromDigitChar(&value, '9', 8);

  assert(value_convert_result == NULL);
}

static void FromStrToInt_Decimal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "42";
  int value;
  int* result;

  result = Integer_FromStrToInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == 42);
}

static void FromStrToInt_NegativeDecimal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "-42";
  int value;
  int* result;

  result = Integer_FromStrToInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == -42);
}

static void FromStrToInt_MaxDecimal_Converted(
    struct EachContext* context) {
  enum {
    kStrCapacity = 128
  };
  char str[kStrCapacity];
  int value;
  int* result;

  _snprintf(str, kStrCapacity, "%d", INT_MAX);
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToInt(&value, str, strlen(str));

  assert(result != NULL);
  assert(value == INT_MAX);
}

static void FromStrToInt_MinDecimal_Converted(
    struct EachContext* context) {
  enum {
    kStrCapacity = 128
  };
  char str[kStrCapacity];
  int value;
  int* result;

  _snprintf(str, kStrCapacity, "%d", INT_MIN);
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToInt(&value, str, strlen(str));

  assert(result != NULL);
  assert(value == INT_MIN);
}

static void FromStrToInt_Octal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "042";
  int value;
  int* result;

  result = Integer_FromStrToInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == 042);
}

static void FromStrToInt_NegativeOctal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "-042";
  int value;
  int* result;

  result = Integer_FromStrToInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == -042);
}

static void FromStrToInt_MaxOctal_Converted(
    struct EachContext* context) {
  enum {
    kStrCapacity = 128
  };
  char str[kStrCapacity];
  int value;
  int* result;

  _snprintf(str, kStrCapacity, "0%o", INT_MAX);
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToInt(&value, str, strlen(str));

  assert(result != NULL);
  assert(value == INT_MAX);
}

static void FromStrToInt_MinOctal_Converted(
    struct EachContext* context) {
  enum {
    kStrCapacity = 128
  };
  char str[kStrCapacity];
  int value;
  int* result;

  _snprintf(str, kStrCapacity, "-0%o", INT_MIN);
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToInt(&value, str, strlen(str));

  assert(result != NULL);
  assert(value == INT_MIN);
}

static void FromStrToInt_Hexadecimal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "0x42";
  int value;
  int* result;

  result = Integer_FromStrToInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == 0x42);
}

static void FromStrToInt_NegativeHexadecimal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "-0x42";
  int value;
  int* result;

  result = Integer_FromStrToInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == -0x42);
}

static void FromStrToInt_MaxHexadecimal_Converted(
    struct EachContext* context) {
  enum {
    kStrCapacity = 128
  };
  char str[kStrCapacity];
  int value;
  int* result;

  _snprintf(str, kStrCapacity, "0x%X", INT_MAX);
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToInt(&value, str, strlen(str));

  assert(result != NULL);
  assert(value == INT_MAX);
}

static void FromStrToInt_MinHexadecimal_Converted(
    struct EachContext* context) {
  enum {
    kStrCapacity = 128
  };
  char str[kStrCapacity];
  int value;
  int* result;

  _snprintf(str, kStrCapacity, "-0x%x", INT_MIN);
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToInt(&value, str, strlen(str));

  assert(result != NULL);
  assert(value == INT_MIN);
}

static void FromStrToInt_Empty_ReturnsNull(
    struct EachContext* context) {
  int value;
  int* result;

  result = Integer_FromStrToInt(&value, "", 0);

  assert(result == NULL);
}

static void FromStrToInt_InvalidChars_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "Hello world!";
  int value;
  int* result;

  result = Integer_FromStrToInt(&value, kStr, strlen(kStr));

  assert(result == NULL);
}

static void FromStrToInt_NotInDecimalBase_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "1000a";
  int value;
  int* result;

  result = Integer_FromStrToInt(&value, kStr, strlen(kStr));

  assert(result == NULL);
}

static void FromStrToInt_NotInOctalBase_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "0888";
  int value;
  int* result;

  result = Integer_FromStrToInt(&value, kStr, strlen(kStr));

  assert(result == NULL);
}

static void FromStrToInt_NotInHexadecimalBase_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "0x00G";
  int value;
  int* result;

  result = Integer_FromStrToInt(&value, kStr, strlen(kStr));

  assert(result == NULL);
}

static void FromStrToInt_OutOfMinRange_ReturnsNull(
    struct EachContext* context) {
  enum {
    kStrCapacity = 1024
  };
  size_t i;
  char str[kStrCapacity];
  int value;
  int* result;

  memset(str, '9', kStrCapacity - 1);
  str[0] = '-';
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToInt(&value, str, strlen(str));

  assert(result == NULL);
}

static void FromStrToInt_OutOfMaxRange_ReturnsNull(
    struct EachContext* context) {
  enum {
    kStrCapacity = 1024
  };
  size_t i;
  char str[kStrCapacity];
  int value;
  int* result;

  memset(str, '9', kStrCapacity - 1);
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToInt(&value, str, strlen(str));

  assert(result == NULL);
}

static void FromStrToUInt_Decimal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "42";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == 42u);
}

static void FromStrToUInt_MaxDecimal_Converted(
    struct EachContext* context) {
  enum {
    kStrCapacity = 128
  };
  char str[kStrCapacity];
  int value;
  int* result;

  _snprintf(str, kStrCapacity, "%u", UINT_MAX);
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToUInt(&value, str, strlen(str));

  assert(result != NULL);
  assert(value == UINT_MAX);
}

static void FromStrToUInt_MinDecimal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "0";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == 0u);
}

static void FromStrToUInt_Octal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "042";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == 042u);
}

static void FromStrToUInt_MaxOctal_Converted(
    struct EachContext* context) {
  enum {
    kStrCapacity = 128
  };
  char str[kStrCapacity];
  int value;
  int* result;

  _snprintf(str, kStrCapacity, "0%o", UINT_MAX);
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToUInt(&value, str, strlen(str));

  assert(result != NULL);
  assert(value == UINT_MAX);
}

static void FromStrToUInt_MinOctal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "0";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == 0);
}

static void FromStrToUInt_Hexadecimal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "0x42";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == 0x42);
}

static void FromStrToUInt_MaxHexadecimal_Converted(
    struct EachContext* context) {
  enum {
    kStrCapacity = 128
  };
  char str[kStrCapacity];
  int value;
  int* result;

  _snprintf(str, kStrCapacity, "0x%X", INT_MAX);
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToUInt(&value, str, strlen(str));

  assert(result != NULL);
  assert(value == INT_MAX);
}

static void FromStrToUInt_MinHexadecimal_Converted(
    struct EachContext* context) {
  static const char kStr[] = "0";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result != NULL);
  assert(value == 0);
}

static void FromStrToUInt_Empty_ReturnsNull(
    struct EachContext* context) {
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, "", 0);

  assert(result == NULL);
}

static void FromStrToUInt_InvalidChars_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "Hello world!";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result == NULL);
}

static void FromStrToUInt_NegativeDecimal_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "-42";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result == NULL);
}

static void FromStrToUInt_NegativeOctal_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "-042";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result == NULL);
}

static void FromStrToUInt_NegativeHexadecimal_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "-0x42";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result == NULL);
}

static void FromStrToUInt_NotInDecimalBase_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "1000a";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result == NULL);
}

static void FromStrToUInt_NotInOctalBase_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "0888";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result == NULL);
}

static void FromStrToUInt_NotInHexadecimalBase_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "0x00G";
  int value;
  int* result;

  result = Integer_FromStrToUInt(&value, kStr, strlen(kStr));

  assert(result == NULL);
}

static void FromStrToUInt_OutOfMinRange_ReturnsNull(
    struct EachContext* context) {
  enum {
    kStrCapacity = 1024
  };
  size_t i;
  char str[kStrCapacity];
  int value;
  int* result;

  memset(str, '9', kStrCapacity - 1);
  str[0] = '-';
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToUInt(&value, str, strlen(str));

  assert(result == NULL);
}

static void FromStrToUInt_OutOfMaxRange_ReturnsNull(
    struct EachContext* context) {
  enum {
    kStrCapacity = 1024
  };
  size_t i;
  char str[kStrCapacity];
  int value;
  int* result;

  memset(str, '9', kStrCapacity - 1);
  str[kStrCapacity - 1] = '\0';

  result = Integer_FromStrToUInt(&value, str, strlen(str));

  assert(result == NULL);
}

static void GetBaseFromPrefixStr_Octal_Base8(struct EachContext* context) {
  static const char kStr[] = "042";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, strlen(kStr));

  assert(base_get_result != NULL);
  assert(base == 8);
}

static void GetBaseFromPrefixStr_NegativeOctal_Base8(
    struct EachContext* context) {
  static const char kStr[] = "-042";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, strlen(kStr));

  assert(base_get_result != NULL);
  assert(base == 8);
}

static void GetBaseFromPrefixStr_OctalZero_Base8(struct EachContext* context) {
  static const char kStr[] = "00";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, strlen(kStr));

  assert(base_get_result != NULL);
  assert(base == 8);
}

static void GetBaseFromPrefixStr_OctalNegativeZero_Base8(
    struct EachContext* context) {
  static const char kStr[] = "-00";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, strlen(kStr));

  assert(base_get_result != NULL);
  assert(base == 8);
}

static void GetBaseFromPrefixStr_Decimal_Base10(struct EachContext* context) {
  static const char kStr[] = "42";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, strlen(kStr));

  assert(base_get_result != NULL);
  assert(base == 10);
}

static void GetBaseFromPrefixStr_NegativeDecimal_Base10(
    struct EachContext* context) {
  static const char kStr[] = "-42";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, strlen(kStr));

  assert(base_get_result != NULL);
  assert(base == 10);
}

static void GetBaseFromPrefixStr_Zero_Base10(struct EachContext* context) {
  static const char kStr[] = "0";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, strlen(kStr));

  assert(base_get_result != NULL);
  assert(base == 10);
}

static void GetBaseFromPrefixStr_NegativeZero_Base10(
    struct EachContext* context) {
  static const char kStr[] = "-0";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, strlen(kStr));

  assert(base_get_result != NULL);
  assert(base == 10);
}

static void GetBaseFromPrefixStr_Hexadecimal_Base16(
    struct EachContext* context) {
  static const char kStr[] = "0x42";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, strlen(kStr));

  assert(base_get_result != NULL);
  assert(base == 16);
}

static void GetBaseFromPrefixStr_NegativeHexadecimal_Base16(
    struct EachContext* context) {
  static const char kStr[] = "-0x42";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, strlen(kStr));

  assert(base_get_result != NULL);
  assert(base == 16);
}

static void GetBaseFromPrefixStr_Empty_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, 0);

  assert(base_get_result == NULL);
}

static void GetBaseFromPrefixStr_Missing0Hexadecimal_ReturnsNull(
    struct EachContext* context) {
  static const char kStr[] = "x42";

  int base;
  int* base_get_result;

  base_get_result = Integer_GetBaseFromPrefixStr(&base, kStr, strlen(kStr));

  assert(base_get_result == NULL);
}

static void IsDigitCharOfBase_DigitChars_ReturnsNonZero(
    struct EachContext* context) {
  size_t i;

  for (i = 0; i < kDigitCharCount; ++i) {
    int result;

    result = Integer_IsDigitCharOfBase(kDigitChars[i], 36);

    assert(result);
  }
}

static void IsDigitCharOfBase_Base0_ReturnsZero(
    struct EachContext* context) {
  int result;

  result = Integer_IsDigitCharOfBase('0', 0);

  assert(!result);
}

static void IsDigitCharOfBase_AboveBase36_ReturnsZero(
    struct EachContext* context) {
  int result;

  result = Integer_IsDigitCharOfBase('0', 37);

  assert(!result);
}

static void IsDigitCharOfBase_OutOfBaseRange_ReturnsZero(
    struct EachContext* context) {
  int result;

  result = Integer_IsDigitCharOfBase('8', 8);

  assert(!result);
}

static void IsStrOfBase_OctalBase8_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "042";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(result);
}

static void IsStrOfBase_NegativeOctalBase8_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "-042";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(result);
}

static void IsStrOfBase_OctalZeroBase8_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "00";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(result);
}

static void IsStrOfBase_OctalNegativeZeroBase8_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "-00";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(result);
}

static void IsStrOfBase_PrefixlessOctalBase8_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(result);
}

static void IsStrOfBase_PrefixlessNegativeOctalBase8_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "-42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(result);
}

static void IsStrOfBase_PrefixlessZeroBase8_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "0";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(result);
}

static void IsStrOfBase_PrefixlessNegativeZeroBase8_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "-0";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(result);
}

static void IsStrOfBase_NonOctalBase8_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "08";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(!result);
}

static void IsStrOfBase_NegativeNonOctalBase8_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "-08";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(!result);
}

static void IsStrOfBase_HexadecimalBase8_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "-0x42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(!result);
}

static void IsStrOfBase_NegativeHexadecimalBase8_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "-0x42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 8);

  assert(!result);
}

static void IsStrOfBase_DecimalBase10_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 10);

  assert(result);
}

static void IsStrOfBase_NegativeDecimalBase10_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "-42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 10);

  assert(result);
}

static void IsStrOfBase_DecimalZeroBase10_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "0";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 10);

  assert(result);
}

static void IsStrOfBase_DecimalNegativeZeroBase10_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "-0";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 10);

  assert(result);
}

static void IsStrOfBase_OctalBase10_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "042";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 10);

  assert(!result);
}

static void IsStrOfBase_NegativeOctalBase10_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "-042";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 10);

  assert(!result);
}

static void IsStrOfBase_HexadecimalBase10_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "0x42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 10);

  assert(!result);
}

static void IsStrOfBase_NegativeHexadecimalBase10_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "-0x42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 10);

  assert(!result);
}

static void IsStrOfBase_NonDecimalBase10_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "a";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 10);

  assert(!result);
}

static void IsStrOfBase_NegativeNonDecimalBase10_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "-a";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 10);

  assert(!result);
}

static void IsStrOfBase_HexadecimalBase16_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "0x42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(result);
}

static void IsStrOfBase_NegativeHexadecimalBase16_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "-0x42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(result);
}

static void IsStrOfBase_HexadecimalZeroBase16_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "0x0";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(result);
}

static void IsStrOfBase_HexadecimalNegativeZeroBase8_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "-0x0";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(result);
}

static void IsStrOfBase_PrefixlessHexadecimalBase16_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(result);
}

static void IsStrOfBase_PrefixlessNegativeHexadecimalBase16_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "-42";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(result);
}

static void IsStrOfBase_PrefixlessZeroBase16_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "0";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(result);
}

static void IsStrOfBase_PrefixlessNegativeZeroBase16_ReturnsNonZero(
    struct EachContext* context) {
  static const char kStr[] = "-0";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(result);
}

static void IsStrOfBase_OctalBase16_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "042";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(!result);
}

static void IsStrOfBase_NegativeOctalBase16_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "-042";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(!result);
}

static void IsStrOfBase_NonHexadecimalBase16_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "0xG";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(!result);
}

static void IsStrOfBase_NegativeNonHexadecimalBase16_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "-0xG";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(!result);
}

static void IsStrOfBase_OnlyHexdecimalPrefixBase16_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "0x";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(!result);
}

static void IsStrOfBase_OnlyNegativeHexdecimalPrefixBase16_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "-0x";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 16);

  assert(!result);
}

static void IsStrOfBase_Empty_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 10);

  assert(!result);
}

static void IsStrOfBase_Base0_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "0";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 0);

  assert(!result);
}

static void IsStrOfBase_BaseAbove36_ReturnsZero(
    struct EachContext* context) {
  static const char kStr[] = "0";
  int result;

  result = Integer_IsStrOfBase(kStr, strlen(kStr), 37);

  assert(!result);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &FromDigitChar_DigitChars_Converted,
    &FromDigitChar_InvalidBase_ReturnsNull,
    &FromDigitChar_OutOfBaseRange_ReturnsNull,

    &FromStrToInt_Decimal_Converted,
    &FromStrToInt_NegativeDecimal_Converted,
    &FromStrToInt_MaxDecimal_Converted,
    &FromStrToInt_MinDecimal_Converted,
    &FromStrToInt_Octal_Converted,
    &FromStrToInt_NegativeOctal_Converted,
    &FromStrToInt_MaxOctal_Converted,
    &FromStrToInt_MinOctal_Converted,
    &FromStrToInt_Hexadecimal_Converted,
    &FromStrToInt_NegativeHexadecimal_Converted,
    &FromStrToInt_MaxHexadecimal_Converted,
    &FromStrToInt_MinHexadecimal_Converted,
    &FromStrToInt_Empty_ReturnsNull,
    &FromStrToInt_InvalidChars_ReturnsNull,
    &FromStrToInt_NotInDecimalBase_ReturnsNull,
    &FromStrToInt_NotInOctalBase_ReturnsNull,
    &FromStrToInt_NotInHexadecimalBase_ReturnsNull,
    &FromStrToInt_OutOfMinRange_ReturnsNull,
    &FromStrToInt_OutOfMaxRange_ReturnsNull,

    &FromStrToUInt_Decimal_Converted,
    &FromStrToUInt_MaxDecimal_Converted,
    &FromStrToUInt_MinDecimal_Converted,
    &FromStrToUInt_Octal_Converted,
    &FromStrToUInt_MaxOctal_Converted,
    &FromStrToUInt_MinOctal_Converted,
    &FromStrToUInt_Hexadecimal_Converted,
    &FromStrToUInt_MaxHexadecimal_Converted,
    &FromStrToUInt_MinHexadecimal_Converted,
    &FromStrToUInt_Empty_ReturnsNull,
    &FromStrToUInt_InvalidChars_ReturnsNull,
    &FromStrToUInt_NegativeDecimal_ReturnsNull,
    &FromStrToUInt_NegativeOctal_ReturnsNull,
    &FromStrToUInt_NegativeHexadecimal_ReturnsNull,
    &FromStrToUInt_NotInDecimalBase_ReturnsNull,
    &FromStrToUInt_NotInOctalBase_ReturnsNull,
    &FromStrToUInt_NotInHexadecimalBase_ReturnsNull,
    &FromStrToUInt_OutOfMinRange_ReturnsNull,
    &FromStrToUInt_OutOfMaxRange_ReturnsNull,

    &GetBaseFromPrefixStr_Octal_Base8,
    &GetBaseFromPrefixStr_NegativeOctal_Base8,
    &GetBaseFromPrefixStr_OctalZero_Base8,
    &GetBaseFromPrefixStr_OctalNegativeZero_Base8,
    &GetBaseFromPrefixStr_Decimal_Base10,
    &GetBaseFromPrefixStr_NegativeDecimal_Base10,
    &GetBaseFromPrefixStr_Zero_Base10,
    &GetBaseFromPrefixStr_NegativeZero_Base10,
    &GetBaseFromPrefixStr_Hexadecimal_Base16,
    &GetBaseFromPrefixStr_NegativeHexadecimal_Base16,
    &GetBaseFromPrefixStr_Empty_ReturnsNull,
    &GetBaseFromPrefixStr_Missing0Hexadecimal_ReturnsNull,

    &IsDigitCharOfBase_DigitChars_ReturnsNonZero,
    &IsDigitCharOfBase_Base0_ReturnsZero,
    &IsDigitCharOfBase_AboveBase36_ReturnsZero,
    &IsDigitCharOfBase_OutOfBaseRange_ReturnsZero,

    &IsStrOfBase_OctalBase8_ReturnsNonZero,
    &IsStrOfBase_NegativeOctalBase8_ReturnsNonZero,
    &IsStrOfBase_OctalZeroBase8_ReturnsNonZero,
    &IsStrOfBase_OctalNegativeZeroBase8_ReturnsNonZero,
    &IsStrOfBase_PrefixlessOctalBase8_ReturnsNonZero,
    &IsStrOfBase_PrefixlessNegativeOctalBase8_ReturnsNonZero,
    &IsStrOfBase_PrefixlessZeroBase8_ReturnsNonZero,
    &IsStrOfBase_PrefixlessNegativeZeroBase8_ReturnsNonZero,
    &IsStrOfBase_NonOctalBase8_ReturnsZero,
    &IsStrOfBase_NegativeNonOctalBase8_ReturnsZero,
    &IsStrOfBase_HexadecimalBase8_ReturnsZero,
    &IsStrOfBase_NegativeHexadecimalBase8_ReturnsZero,
    &IsStrOfBase_DecimalBase10_ReturnsNonZero,
    &IsStrOfBase_NegativeDecimalBase10_ReturnsNonZero,
    &IsStrOfBase_DecimalZeroBase10_ReturnsNonZero,
    &IsStrOfBase_DecimalNegativeZeroBase10_ReturnsNonZero,
    &IsStrOfBase_OctalBase10_ReturnsZero,
    &IsStrOfBase_NegativeOctalBase10_ReturnsZero,
    &IsStrOfBase_HexadecimalBase10_ReturnsZero,
    &IsStrOfBase_NegativeHexadecimalBase10_ReturnsZero,
    &IsStrOfBase_NonDecimalBase10_ReturnsZero,
    &IsStrOfBase_NegativeNonDecimalBase10_ReturnsZero,
    &IsStrOfBase_HexadecimalBase16_ReturnsNonZero,
    &IsStrOfBase_NegativeHexadecimalBase16_ReturnsNonZero,
    &IsStrOfBase_HexadecimalZeroBase16_ReturnsNonZero,
    &IsStrOfBase_HexadecimalNegativeZeroBase8_ReturnsNonZero,
    &IsStrOfBase_PrefixlessHexadecimalBase16_ReturnsNonZero,
    &IsStrOfBase_PrefixlessNegativeHexadecimalBase16_ReturnsNonZero,
    &IsStrOfBase_PrefixlessZeroBase16_ReturnsNonZero,
    &IsStrOfBase_PrefixlessNegativeZeroBase16_ReturnsNonZero,
    &IsStrOfBase_OctalBase16_ReturnsZero,
    &IsStrOfBase_NegativeOctalBase16_ReturnsZero,
    &IsStrOfBase_NonHexadecimalBase16_ReturnsZero,
    &IsStrOfBase_NegativeNonHexadecimalBase16_ReturnsZero,
    &IsStrOfBase_OnlyHexdecimalPrefixBase16_ReturnsZero,
    &IsStrOfBase_OnlyNegativeHexdecimalPrefixBase16_ReturnsZero,
    &IsStrOfBase_Empty_ReturnsZero,
    &IsStrOfBase_Base0_ReturnsZero,
    &IsStrOfBase_BaseAbove36_ReturnsZero
  };

  enum {
    kTestsCount = sizeof(kTests) / sizeof(kTests[0])
  };

  size_t i;

  BeforeAllSetUp();

  printf("Running %u test(s).\n", kTestsCount);
  for (i = 0; i < kTestsCount; ++i) {
    struct EachContext context;

    BeforeEach(&context);
    kTests[i](&context);
    AfterEach(&context);
  }
  printf("Ran %u test(s).\n", kTestsCount);

#endif

  return 0;
}
