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

#include "bh/config/colonini/type/value.h"
#include "bh/config/colonini/type/value_type.h"

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
  struct Colonini_Value value;
  struct Colonini_Value* value_init_result;
  struct Colonini_Data* data;

  value_init_result = Colonini_Value_InitAsBoolean(&value, kExpected);

  assert(value_init_result != NULL);
  assert(value.type == Colonini_ValueType_kData);
  data = &value.variant.as_data;
  assert(data->type == Colonini_DataType_kBoolean);
  assert(data->variant.as_boolean == kExpected);

  Colonini_Value_Deinit(&value);
}

static void InitAsBoolean_True_IsTrue(struct EachContext* context) {
  enum {
    kExpected = 1
  };
  struct Colonini_Value value;
  struct Colonini_Value* value_init_result;
  struct Colonini_Data* data;

  value_init_result = Colonini_Value_InitAsBoolean(&value, kExpected);

  assert(value_init_result != NULL);
  assert(value.type == Colonini_ValueType_kData);
  data = &value.variant.as_data;
  assert(data->type == Colonini_DataType_kBoolean);
  assert(data->variant.as_boolean == kExpected);

  Colonini_Value_Deinit(&value);
}

static void InitAsEmpty_IsEmpty(struct EachContext* context) {
  struct Colonini_Value value;
  struct Colonini_Value* value_init_result;

  value_init_result = Colonini_Value_InitAsEmpty(&value);

  assert(value_init_result != NULL);
  assert(value.type == Colonini_ValueType_kEmpty);
}

static void InitAsInteger_Zero_IsZero(struct EachContext* context) {
  enum {
    kExpected = 0
  };
  struct Colonini_Value value;
  struct Colonini_Value* value_init_result;
  struct Colonini_Data* data;

  value_init_result = Colonini_Value_InitAsInteger(&value, kExpected);

  assert(value_init_result != NULL);
  assert(value.type == Colonini_ValueType_kData);
  data = &value.variant.as_data;
  assert(data->type == Colonini_DataType_kInteger);
  assert(data->variant.as_integer == kExpected);

  Colonini_Value_Deinit(&value);
}

static void InitAsInteger_42_Is42(struct EachContext* context) {
  enum {
    kExpected = 42
  };
  struct Colonini_Value value;
  struct Colonini_Value* value_init_result;
  struct Colonini_Data* data;

  value_init_result = Colonini_Value_InitAsInteger(&value, kExpected);

  assert(value_init_result != NULL);
  assert(value.type == Colonini_ValueType_kData);
  data = &value.variant.as_data;
  assert(data->type == Colonini_DataType_kInteger);
  assert(data->variant.as_integer == kExpected);

  Colonini_Value_Deinit(&value);
}

static void InitAsMap_IsEmptyMap(struct EachContext* context) {
  struct Colonini_Value value;
  struct Colonini_Value* value_init_result;
  struct Colonini_Map* map;

  value_init_result = Colonini_Value_InitAsMap(&value);

  assert(value_init_result != NULL);
  assert(value.type == Colonini_ValueType_kMap);
  map = &value.variant.as_map;
  assert(map->count == 0);

  Colonini_Value_Deinit(&value);
}

static void InitAsString_Empty_IsEmpty(struct EachContext* context) {
  static const char kExpected[] = "";
  enum {
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };
  struct Colonini_Value value;
  struct Colonini_Value* value_init_result;
  struct Colonini_Data* data;
  struct Colonini_String* str;

  value_init_result =
      Colonini_Value_InitAsString(&value, kExpected, kExpectedLength);

  assert(value_init_result != NULL);
  assert(value.type == Colonini_ValueType_kData);
  data = &value.variant.as_data;
  assert(data->type == Colonini_DataType_kString);
  str = &data->variant.as_string;
  assert(str->length == kExpectedLength);
  assert(memcmp(str->str, kExpected, str->length) == 0);

  Colonini_Value_Deinit(&value);
}

static void InitAsString_Hello_EqualText(struct EachContext* context) {
  static const char kExpected[] = "Hello";
  enum {
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };
  struct Colonini_Value value;
  struct Colonini_Value* value_init_result;
  struct Colonini_Data* data;
  struct Colonini_String* str;

  value_init_result =
      Colonini_Value_InitAsString(&value, kExpected, kExpectedLength);

  assert(value_init_result != NULL);
  assert(value.type == Colonini_ValueType_kData);
  data = &value.variant.as_data;
  assert(data->type == Colonini_DataType_kString);
  str = &data->variant.as_string;
  assert(str->length == kExpectedLength);
  assert(memcmp(str->str, kExpected, str->length) == 0);

  Colonini_Value_Deinit(&value);
}

static void InitAsToggle_Enabled_IsEnabled(struct EachContext* context) {
  struct Colonini_Value value;
  struct Colonini_Value* value_init_result;
  struct Colonini_Data* data;
  struct Colonini_Toggle* toggle;

  value_init_result = Colonini_Value_InitAsToggle(&value, 1, 0);

  assert(value_init_result != NULL);
  assert(value.type == Colonini_ValueType_kData);
  data = &value.variant.as_data;
  assert(data->type == Colonini_DataType_kToggle);
  toggle = &data->variant.as_toggle;
  assert(toggle->enabled);

  Colonini_Value_Deinit(&value);
}

static void InitAsToggle_Disabled_IsDisabled(struct EachContext* context) {
  struct Colonini_Value value;
  struct Colonini_Value* value_init_result;
  struct Colonini_Data* data;
  struct Colonini_Toggle* toggle;

  value_init_result = Colonini_Value_InitAsToggle(&value, 0, 0);

  assert(value_init_result != NULL);
  assert(value.type == Colonini_ValueType_kData);
  data = &value.variant.as_data;
  assert(data->type == Colonini_DataType_kToggle);
  toggle = &data->variant.as_toggle;
  assert(!toggle->enabled);

  Colonini_Value_Deinit(&value);
}

static void InitAsToggle_Key_SetKeyCode(struct EachContext* context) {
  struct Colonini_Value value;
  struct Colonini_Value* value_init_result;
  struct Colonini_Data* data;
  struct Colonini_Toggle* toggle;

  value_init_result = Colonini_Value_InitAsToggle(&value, 0, 42);

  assert(value_init_result != NULL);
  assert(value.type == Colonini_ValueType_kData);
  data = &value.variant.as_data;
  assert(data->type == Colonini_DataType_kToggle);
  toggle = &data->variant.as_toggle;
  assert(toggle->key_code == 42);

  Colonini_Value_Deinit(&value);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &InitAsBoolean_False_IsFalse,
    &InitAsBoolean_True_IsTrue,

    &InitAsEmpty_IsEmpty,

    &InitAsInteger_Zero_IsZero,
    &InitAsInteger_42_Is42,

    &InitAsMap_IsEmptyMap,

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
