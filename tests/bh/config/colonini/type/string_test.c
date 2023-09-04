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
#include <string.h>

#include "bh/config/colonini/type/string.h"

static void BeforeAllSetUp(void) {}

struct EachContext {
  unsigned char unused;
};

typedef void TestFunc(struct EachContext* context);

static void BeforeEach(struct EachContext* context) {}

static void AfterEach(struct EachContext* context) {}

static void Init_Empty_IsEmpty(struct EachContext* context) {
  static const char kExpected[] = "";
  enum {
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  struct Colonini_String str;
  struct Colonini_String* str_init_result;

  str_init_result = Colonini_String_Init(&str, kExpected, kExpectedLength);

  assert(str_init_result != NULL);
  assert(str.length == kExpectedLength);
  assert(memcmp(str.str, kExpected, kExpectedLength) == 0);

  Colonini_String_Deinit(&str);
}

static void Init_Hello_EqualText(struct EachContext* context) {
  static const char kExpected[] = "Hello";
  enum {
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  struct Colonini_String str;
  struct Colonini_String* str_init_result;

  str_init_result = Colonini_String_Init(&str, kExpected, kExpectedLength);

  assert(str_init_result != NULL);
  assert(str.length == kExpectedLength);
  assert(memcmp(str.str, kExpected, kExpectedLength) == 0);

  Colonini_String_Deinit(&str);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &Init_Empty_IsEmpty,
    &Init_Hello_EqualText
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
