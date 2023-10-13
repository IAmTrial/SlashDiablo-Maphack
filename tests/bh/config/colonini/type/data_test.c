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

#include "bh/config/colonini/type/data.h"

static void BeforeAllSetUp(void) {}

struct EachContext {
  unsigned char unused;
};

typedef void TestFunc(struct EachContext* context);

static void BeforeEach(struct EachContext* context) {}

static void AfterEach(struct EachContext* context) {}

static void InitAsBoolean_False_IsFalse(struct EachContext* context) {
  enum {
    kExpected = 0
  };

  struct Colonini_Data data;
  struct Colonini_Data* data_init_result;

  data_init_result = Colonini_Data_InitAsBoolean(&data, kExpected);

  assert(data_init_result != NULL);
  assert(data.type == Colonini_DataType_kBoolean);
  assert(data.variant.as_boolean == kExpected);

  Colonini_Data_Deinit(&data);
}

static void InitAsBoolean_True_IsTrue(struct EachContext* context) {
  enum {
    kExpected = 1
  };

  struct Colonini_Data data;
  struct Colonini_Data* data_init_result;

  data_init_result = Colonini_Data_InitAsBoolean(&data, kExpected);

  assert(data_init_result != NULL);
  assert(data.type == Colonini_DataType_kBoolean);
  assert(data.variant.as_boolean == kExpected);

  Colonini_Data_Deinit(&data);
}

static void InitAsInteger_Zero_IsZero(struct EachContext* context) {
  enum {
    kExpected = 0
  };

  struct Colonini_Data data;
  struct Colonini_Data* data_init_result;

  data_init_result = Colonini_Data_InitAsInteger(&data, kExpected);

  assert(data_init_result != NULL);
  assert(data.type == Colonini_DataType_kInteger);
  assert(data.variant.as_integer == kExpected);

  Colonini_Data_Deinit(&data);
}

static void InitAsInteger_42_Is42(struct EachContext* context) {
  enum {
    kExpected = 42
  };

  struct Colonini_Data data;
  struct Colonini_Data* data_init_result;

  data_init_result = Colonini_Data_InitAsInteger(&data, kExpected);

  assert(data_init_result != NULL);
  assert(data.type == Colonini_DataType_kInteger);
  assert(data.variant.as_integer == kExpected);

  Colonini_Data_Deinit(&data);
}

static void InitAsString_Empty_IsEmpty(struct EachContext* context) {
  static const char kExpected[] = "";
  enum {
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  struct Colonini_Data data;
  struct Colonini_Data* data_init_result;
  struct Colonini_String* str;

  data_init_result = Colonini_Data_InitAsString(&data, kExpected, kExpectedLength);

  assert(data_init_result != NULL);
  assert(data.type == Colonini_DataType_kString);
  str = &data.variant.as_string;
  assert(str->length == kExpectedLength);
  assert(memcmp(str->str, kExpected, kExpectedLength) == 0);

  Colonini_Data_Deinit(&data);
}

static void InitAsString_Hello_EqualText(struct EachContext* context) {
  static const char kExpected[] = "Hello";
  enum {
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  struct Colonini_Data data;
  struct Colonini_Data* data_init_result;
  struct Colonini_String* str;

  data_init_result = Colonini_Data_InitAsString(&data, kExpected, kExpectedLength);

  assert(data_init_result != NULL);
  assert(data.type == Colonini_DataType_kString);
  str = &data.variant.as_string;
  assert(str->length == kExpectedLength);
  assert(memcmp(str->str, kExpected, kExpectedLength) == 0);

  Colonini_Data_Deinit(&data);
}

static void InitAsToggle_Enabled_IsEnabled(struct EachContext* context) {
  struct Colonini_Data data;
  struct Colonini_Data* data_init_result;
  struct Colonini_Toggle* toggle;

  data_init_result = Colonini_Data_InitAsToggle(&data, 1, 0);

  assert(data_init_result != NULL);
  assert(data.type == Colonini_DataType_kToggle);
  toggle = &data.variant.as_toggle;
  assert(toggle->enabled);

  Colonini_Data_Deinit(&data);
}

static void InitAsToggle_Disabled_IsDisabled(struct EachContext* context) {
  struct Colonini_Data data;
  struct Colonini_Data* data_init_result;
  struct Colonini_Toggle* toggle;

  data_init_result = Colonini_Data_InitAsToggle(&data, 0, 0);

  assert(data_init_result != NULL);
  assert(data.type == Colonini_DataType_kToggle);
  toggle = &data.variant.as_toggle;
  assert(!toggle->enabled);

  Colonini_Data_Deinit(&data);
}

static void InitAsToggle_Key_SetKeyCode(struct EachContext* context) {
  struct Colonini_Data data;
  struct Colonini_Data* data_init_result;
  struct Colonini_Toggle* toggle;

  data_init_result = Colonini_Data_InitAsToggle(&data, 0, 42);

  assert(data_init_result != NULL);
  assert(data.type == Colonini_DataType_kToggle);
  toggle = &data.variant.as_toggle;
  assert(toggle->key_code == 42);

  Colonini_Data_Deinit(&data);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &InitAsBoolean_False_IsFalse,
    &InitAsBoolean_True_IsTrue,

    &InitAsInteger_Zero_IsZero,
    &InitAsInteger_42_Is42,

    &InitAsString_Empty_IsEmpty,
    &InitAsString_Hello_EqualText,

    &InitAsToggle_Enabled_IsEnabled,
    &InitAsToggle_Disabled_IsDisabled,
    &InitAsToggle_Key_SetKeyCode
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
