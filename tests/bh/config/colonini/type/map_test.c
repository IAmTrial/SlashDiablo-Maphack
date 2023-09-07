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

static const char kKey[] = "key";
enum {
  kKeyLength = sizeof(kKey) / sizeof(kKey[0])
};

static void BeforeAllSetUp(void) {}

struct EachContext {
  struct Colonini_Map map;
};

typedef void TestFunc(struct EachContext* context);

static void BeforeEach(struct EachContext* context) {
  Colonini_Map_Init(&context->map);
}

static void AfterEach(struct EachContext* context) {
  Colonini_Map_Deinit(&context->map);
}

static void Init_Empty_Successful(struct EachContext* context) {
  struct Colonini_Map map;
  struct Colonini_Map* map_init_result;

  map_init_result = Colonini_Map_Init(&map);

  assert(map_init_result != NULL);
  assert(map.count == 0);

  Colonini_Map_Deinit(&map);
}

static void PutBoolean_False_IsFalse(struct EachContext* context) {
  enum {
    kExpected = 0
  };
  unsigned char* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;

  result = Colonini_Map_PutBoolean(&context->map, kKey, kKeyLength, kExpected);

  assert(result != NULL);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kBoolean);
  assert(result == &data->variant.as_boolean);
  assert(data->variant.as_boolean == kExpected);
}

static void PutBoolean_True_IsTrue(struct EachContext* context) {
  enum {
    kExpected = 1
  };
  unsigned char* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;

  result = Colonini_Map_PutBoolean(&context->map, kKey, kKeyLength, kExpected);

  assert(result != NULL);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kBoolean);
  assert(result == &data->variant.as_boolean);
  assert(data->variant.as_boolean == kExpected);
}

static void PutInteger_Zero_IsZero(struct EachContext* context) {
  enum {
    kExpected = 0
  };
  unsigned int* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;

  result = Colonini_Map_PutInteger(&context->map, kKey, kKeyLength, kExpected);

  assert(result != NULL);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kInteger);
  assert(result == &data->variant.as_integer);
  assert(data->variant.as_integer == kExpected);
}

static void PutInteger_42_Is42(struct EachContext* context) {
  enum {
    kExpected = 42
  };
  unsigned int* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;

  result = Colonini_Map_PutInteger(&context->map, kKey, kKeyLength, kExpected);

  assert(result != NULL);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kInteger);
  assert(result == &data->variant.as_integer);
  assert(data->variant.as_integer == kExpected);
}

static void PutString_Empty_IsEmpty(struct EachContext* context) {
  static const char kExpected[] = "";
  enum {
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };
  struct Colonini_String* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;
  struct Colonini_String* str;

  result =
      Colonini_Map_PutString(
          &context->map, kKey, kKeyLength, kExpected, kExpectedLength);

  assert(result != NULL);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kString);
  str = &data->variant.as_string;
  assert(result == str);
  assert(str->length == kExpectedLength);
  assert(memcmp(str->str, kExpected, kExpectedLength) == 0);
}

static void PutString_Hello_EqualText(struct EachContext* context) {
  static const char kExpected[] = "Hello";
  enum {
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };
  struct Colonini_String* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;
  struct Colonini_String* str;

  result =
      Colonini_Map_PutString(
          &context->map, kKey, kKeyLength, kExpected, kExpectedLength);

  assert(result != NULL);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kString);
  str = &data->variant.as_string;
  assert(result == str);
  assert(str->length == kExpectedLength);
  assert(memcmp(str->str, kExpected, kExpectedLength) == 0);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &Init_Empty_Successful,

    &PutBoolean_False_IsFalse,
    &PutBoolean_True_IsTrue,

    &PutInteger_Zero_IsZero,
    &PutInteger_42_Is42,

    &PutString_Empty_IsEmpty,
    &PutString_Hello_EqualText
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
