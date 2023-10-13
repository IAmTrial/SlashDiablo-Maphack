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
#include <stddef.h>
#include <stdio.h>
#include <wchar.h>

#include "bh/common/string_util/bool.h"

static const char kOnStr[] = "oN";
static const wchar_t kOnWStr[] = L"oN";
static const char kOffStr[] = "OfF";
static const wchar_t kOffWStr[] = L"OfF";

static const char kTrueStr[] = "tRuE";
static const wchar_t kTrueWStr[] = L"tRuE";
static const char kFalseStr[] = "FaLsE";
static const wchar_t kFalseWStr[] = L"FaLsE";

static const char kNeitherStr[] = "Neither";
static const wchar_t kNeitherWStr[] = L"Neither";

enum {
  kOnStrLength = sizeof(kOnStr) / sizeof(kOnStr[0]) - 1,
  kOnWStrLength = sizeof(kOnWStr) / sizeof(kOnWStr[0]) - 1,
  kOffStrLength = sizeof(kOffStr) / sizeof(kOffStr[0]) - 1,
  kOffWStrLength = sizeof(kOffWStr) / sizeof(kOffWStr[0]) - 1,

  kTrueStrLength = sizeof(kTrueStr) / sizeof(kTrueStr[0]) - 1,
  kTrueWStrLength = sizeof(kTrueWStr) / sizeof(kTrueWStr[0]) - 1,
  kFalseStrLength = sizeof(kFalseStr) / sizeof(kFalseStr[0]) - 1,
  kFalseWStrLength = sizeof(kFalseWStr) / sizeof(kFalseWStr[0]) - 1,

  kNeitherStrLength = sizeof(kNeitherStr) / sizeof(kNeitherStr[0]) - 1,
  kNeitherWStrLength = sizeof(kNeitherWStr) / sizeof(kNeitherWStr[0]) - 1
};

static void BeforeAllSetUp(void) {}

struct EachContext {
  unsigned char unused;
};

typedef void TestFunc(struct EachContext* context);

static void BeforeEach(struct EachContext* context) {}

static void AfterEach(struct EachContext* context) {}

static void FromOnOffStr_On_True(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromOnOffStr(&value, kOnStr, kOnStrLength);

  assert(result != NULL);
  assert(value);
}

static void FromOnOffStr_Off_False(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromOnOffStr(&value, kOffStr, kOffStrLength);

  assert(result != NULL);
  assert(!value);
}

static void FromOnOffStr_Neither_ReturnsNull(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromOnOffStr(&value, kNeitherStr, kNeitherStrLength);

  assert(result == NULL);
}

static void FromOnOffStr_Null_ReturnsNull(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromOnOffStr(&value, NULL, 0);

  assert(result == NULL);
}

static void FromOnOffWStr_On_True(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromOnOffWStr(&value, kOnWStr, kOnWStrLength);

  assert(result != NULL);
  assert(value);
}

static void FromOnOffWStr_Off_False(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromOnOffWStr(&value, kOffWStr, kOffWStrLength);

  assert(result != NULL);
  assert(!value);
}

static void FromOnOffWStr_Neither_ReturnsNull(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromOnOffWStr(&value, kNeitherWStr, kNeitherWStrLength);

  assert(result == NULL);
}

static void FromOnOffWStr_Null_ReturnsNull(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromOnOffWStr(&value, NULL, 0);

  assert(result == NULL);
}

static void FromTrueFalseStr_True_True(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromTrueFalseStr(&value, kTrueStr, kTrueStrLength);

  assert(result != NULL);
  assert(value);
}

static void FromTrueFalseStr_False_False(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromTrueFalseStr(&value, kFalseStr, kFalseStrLength);

  assert(result != NULL);
  assert(!value);
}

static void FromTrueFalseStr_Neither_ReturnsNull(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromTrueFalseStr(&value, kNeitherStr, kNeitherStrLength);

  assert(result == NULL);
}

static void FromTrueFalseStr_Null_ReturnsNull(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromTrueFalseStr(&value, NULL, 0);

  assert(result == NULL);
}

static void FromTrueFalseWStr_True_True(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromTrueFalseWStr(&value, kTrueWStr, kTrueWStrLength);

  assert(result != NULL);
  assert(value);
}

static void FromTrueFalseWStr_False_False(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromTrueFalseWStr(&value, kFalseWStr, kFalseWStrLength);

  assert(result != NULL);
  assert(!value);
}

static void FromTrueFalseWStr_Neither_ReturnsNull(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromTrueFalseWStr(&value, kNeitherWStr, kNeitherWStrLength);

  assert(result == NULL);
}

static void FromTrueFalseWStr_Null_ReturnsNull(struct EachContext* context) {
  int value;
  int* result;

  result = Bool_FromTrueFalseWStr(&value, NULL, 0);

  assert(result == NULL);
}

static void ToOnOffStr_NonZero_ReturnsOn(struct EachContext* context) {
  size_t length;
  const char* result;

  result = Bool_ToOnOffStr(1, &length);

  assert(length == 2);
  assert(memcmp(result, "on", 3) == 0);
}

static void ToOnOffStr_Zero_ReturnsOff(struct EachContext* context) {
  size_t length;
  const char* result;

  result = Bool_ToOnOffStr(0, &length);

  assert(length == 3);
  assert(memcmp(result, "off", 4) == 0);
}

static void ToOnOffWStr_NonZero_ReturnsOn(struct EachContext* context) {
  size_t length;
  const wchar_t* result;

  result = Bool_ToOnOffWStr(1, &length);

  assert(length == 2);
  assert(wmemcmp(result, L"on", 3) == 0);
}

static void ToOnOffWStr_Zero_ReturnsOff(struct EachContext* context) {
  size_t length;
  const wchar_t* result;

  result = Bool_ToOnOffWStr(0, &length);

  assert(length == 3);
  assert(wmemcmp(result, L"off", 4) == 0);
}

static void ToTrueFalseStr_NonZero_ReturnsTrue(struct EachContext* context) {
  size_t length;
  const char* result;

  result = Bool_ToTrueFalseStr(1, &length);

  assert(length == 4);
  assert(memcmp(result, "true", 5) == 0);
}

static void ToTrueFalseStr_Zero_ReturnsFalse(struct EachContext* context) {
  size_t length;
  const char* result;

  result = Bool_ToTrueFalseStr(0, &length);

  assert(length == 5);
  assert(memcmp(result, "false", 6) == 0);
}

static void ToTrueFalseWStr_NonZero_ReturnsTrue(struct EachContext* context) {
  size_t length;
  const wchar_t* result;

  result = Bool_ToTrueFalseWStr(1, &length);

  assert(length == 4);
  assert(wmemcmp(result, L"true", 5) == 0);
}

static void ToTrueFalseWStr_Zero_ReturnsFalse(struct EachContext* context) {
  size_t length;
  const wchar_t* result;

  result = Bool_ToTrueFalseWStr(0, &length);

  assert(length == 5);
  assert(wmemcmp(result, L"false", 6) == 0);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &FromOnOffStr_On_True,
    &FromOnOffStr_Off_False,
    &FromOnOffStr_Neither_ReturnsNull,
    &FromOnOffStr_Null_ReturnsNull,
    &FromOnOffWStr_On_True,
    &FromOnOffWStr_Off_False,
    &FromOnOffWStr_Neither_ReturnsNull,
    &FromOnOffWStr_Null_ReturnsNull,

    &FromTrueFalseStr_True_True,
    &FromTrueFalseStr_False_False,
    &FromTrueFalseStr_Neither_ReturnsNull,
    &FromTrueFalseStr_Null_ReturnsNull,
    &FromTrueFalseWStr_True_True,
    &FromTrueFalseWStr_False_False,
    &FromTrueFalseWStr_Neither_ReturnsNull,
    &FromTrueFalseWStr_Null_ReturnsNull,

    &ToOnOffStr_NonZero_ReturnsOn,
    &ToOnOffStr_Zero_ReturnsOff,
    &ToOnOffWStr_NonZero_ReturnsOn,
    &ToOnOffWStr_Zero_ReturnsOff,

    &ToTrueFalseStr_NonZero_ReturnsTrue,
    &ToTrueFalseStr_Zero_ReturnsFalse,
    &ToTrueFalseWStr_NonZero_ReturnsTrue,
    &ToTrueFalseWStr_Zero_ReturnsFalse
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
