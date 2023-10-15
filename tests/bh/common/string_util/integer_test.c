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

#include "bh/common/string_util/integer.h"

static const char kDigitChars[] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e',
  'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
  'u', 'v', 'w', 'x', 'y', 'z'
};

enum {
  kDigitCharsCount = sizeof(kDigitChars) / sizeof(kDigitChars[0])
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

  for (i = 0; i < kDigitCharsCount; ++i) {
    int* value_convert_result;
    int value;

    value_convert_result =
        Integer_FromDigitChar(&value, kDigitChars[i], kDigitCharsCount);

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

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &FromDigitChar_DigitChars_Converted,
    &FromDigitChar_InvalidBase_ReturnsNull,
    &FromDigitChar_OutOfBaseRange_ReturnsNull
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
