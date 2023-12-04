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

#include "bh/config/colonini/type/entry.h"
#include "bh/config/colonini/type/map.h"
#include "bh/config/colonini/type/value.h"
#include "bh/config/colonini/type/value_type.h"

static const char kKey[] = "key";
static const char kKey2[] = "key2";
static const char kKey3[] = "key3";
enum {
  kKeyLength = sizeof(kKey) / sizeof(kKey[0]),
  kKey2Length = sizeof(kKey2) / sizeof(kKey2[0]),
  kKey3Length = sizeof(kKey3) / sizeof(kKey3[0])
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
  assert(context->map.count == 1);
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
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kBoolean);
  assert(result == &data->variant.as_boolean);
  assert(data->variant.as_boolean == kExpected);
}

static void PutBoolean_CallTwice_Replace(struct EachContext* context) {
  enum {
    kExpected = 1
  };
  unsigned char* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;

  Colonini_Map_PutBoolean(&context->map, kKey, kKeyLength, 0);
  result = Colonini_Map_PutBoolean(&context->map, kKey, kKeyLength, kExpected);

  assert(result != NULL);
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kBoolean);
  assert(result == &data->variant.as_boolean);
  assert(data->variant.as_boolean == kExpected);
}

static void PutEmpty_IsEmpty(struct EachContext* context) {
  struct Colonini_Value* result;
  struct Colonini_Value* value;

  result = Colonini_Map_PutEmpty(&context->map, kKey, kKeyLength);

  assert(result != NULL);
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kEmpty);
}

static void PutEmpty_CallTwice_IsEmpty(struct EachContext* context) {
  struct Colonini_Value* result;
  struct Colonini_Value* value;

  Colonini_Map_PutEmpty(&context->map, kKey, kKeyLength);
  result = Colonini_Map_PutEmpty(&context->map, kKey, kKeyLength);

  assert(result != NULL);
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kEmpty);
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
  assert(context->map.count == 1);
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
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kInteger);
  assert(result == &data->variant.as_integer);
  assert(data->variant.as_integer == kExpected);
}

static void PutInteger_CallTwice_Replace(struct EachContext* context) {
  enum {
    kExpected = 42
  };
  unsigned int* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;

  Colonini_Map_PutInteger(&context->map, kKey, kKeyLength, 0);
  result = Colonini_Map_PutInteger(&context->map, kKey, kKeyLength, kExpected);

  assert(result != NULL);
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kInteger);
  assert(result == &data->variant.as_integer);
  assert(data->variant.as_integer == kExpected);
}

static void PutMap_IsEmptyMap(struct EachContext* context) {
  struct Colonini_Map* result;
  struct Colonini_Value* value;
  struct Colonini_Map* submap;

  result = Colonini_Map_PutMap(&context->map, kKey, kKeyLength);

  assert(result != NULL);
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kMap);
  submap = &value->variant.as_map;
  assert(result == submap);
  assert(submap->count == 0);
}

static void PutMap_CallTwice_Replace(struct EachContext* context) {
  struct Colonini_Map* result;
  struct Colonini_Value* value;
  struct Colonini_Map* submap;

  Colonini_Map_PutMap(&context->map, kKey, kKeyLength);
  result = Colonini_Map_PutMap(&context->map, kKey, kKeyLength);

  assert(result != NULL);
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kMap);
  submap = &value->variant.as_map;
  assert(result == submap);
  assert(submap->count == 0);
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
  assert(context->map.count == 1);
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
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kString);
  str = &data->variant.as_string;
  assert(result == str);
  assert(str->length == kExpectedLength);
  assert(memcmp(str->str, kExpected, kExpectedLength) == 0);
}

static void PutString_CallTwice_Replace(struct EachContext* context) {
  static const char kHello[] = "Hello";
  static const char kExpected[] = "Expected";
  struct Colonini_String* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;
  struct Colonini_String* str;

  Colonini_Map_PutString(
      &context->map,
      kKey,
      kKeyLength,
      kHello,
      strlen(kHello));
  result =
      Colonini_Map_PutString(
          &context->map,
          kKey,
          kKeyLength,
          kExpected,
          strlen(kExpected));

  assert(result != NULL);
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kString);
  str = &data->variant.as_string;
  assert(result == str);
  assert(str->length == strlen(kExpected));
  assert(strcmp(str->str, kExpected) == 0);
}

static void PutToggle_Enabled_IsEnabled(struct EachContext* context) {
  struct Colonini_Toggle* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;
  struct Colonini_Toggle* toggle;

  result = Colonini_Map_PutToggle(&context->map, kKey, kKeyLength, 1, 0);

  assert(result != NULL);
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kToggle);
  toggle = &data->variant.as_toggle;
  assert(toggle->enabled);
}

static void PutToggle_Disabled_IsDisabled(struct EachContext* context) {
  struct Colonini_Toggle* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;
  struct Colonini_Toggle* toggle;

  result = Colonini_Map_PutToggle(&context->map, kKey, kKeyLength, 0, 0);

  assert(result != NULL);
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kToggle);
  toggle = &data->variant.as_toggle;
  assert(!toggle->enabled);
}

static void PutToggle_Key_SetKeyCode(struct EachContext* context) {
  struct Colonini_Toggle* result;
  struct Colonini_Value* value;
  struct Colonini_Data* data;
  struct Colonini_Toggle* toggle;

  result = Colonini_Map_PutToggle(&context->map, kKey, kKeyLength, 0, 42);

  assert(result != NULL);
  assert(context->map.count == 1);
  value = Colonini_Map_Get(&context->map, kKey, kKeyLength);
  assert(value->type == Colonini_ValueType_kData);
  data = &value->variant.as_data;
  assert(data->type == Colonini_DataType_kToggle);
  toggle = &data->variant.as_toggle;
  assert(toggle->key_code == 42);
}

static void Remove_OneEntryAndExists_Empty(struct EachContext* context) {
  int removed;

  Colonini_Map_PutBoolean(&context->map, kKey, kKeyLength, 1);

  removed = Colonini_Map_Remove(&context->map, kKey, kKeyLength);

  assert(removed);
  assert(context->map.count == 0);
  assert(context->map.head_entry == NULL);
  assert(context->map.tail_entry == NULL);
}

static void Remove_TwoEntriesAndExists_Removed(struct EachContext* context) {
  int removed;

  Colonini_Map_PutBoolean(&context->map, kKey, kKeyLength, 1);
  Colonini_Map_PutBoolean(&context->map, kKey2, kKey2Length, 0);

  removed = Colonini_Map_Remove(&context->map, kKey, kKeyLength);

  assert(removed);
  assert(context->map.count == 1);
  assert(context->map.head_entry == context->map.tail_entry);
  assert(context->map.head_entry->previous == NULL);
  assert(context->map.head_entry->next == NULL);
}

static void Remove_ThreeEntriesAndExists_Removed(struct EachContext* context) {
  int removed;

  Colonini_Map_PutBoolean(&context->map, kKey, kKeyLength, 1);
  Colonini_Map_PutBoolean(&context->map, kKey2, kKey2Length, 1);
  Colonini_Map_PutBoolean(&context->map, kKey3, kKey3Length, 1);

  removed = Colonini_Map_Remove(&context->map, kKey, kKeyLength);

  assert(removed);
  assert(context->map.count == 2);
  assert(context->map.head_entry != NULL);
  assert(context->map.tail_entry != NULL);
  assert(context->map.head_entry->previous == NULL);
  assert(context->map.head_entry->next == context->map.tail_entry);
  assert(context->map.tail_entry->previous == context->map.head_entry);
  assert(context->map.tail_entry->next == NULL);
}

static void Remove_NotExists_DoesNothing(struct EachContext* context) {
  int removed;

  Colonini_Map_PutBoolean(&context->map, kKey, kKeyLength, 1);

  removed = Colonini_Map_Remove(&context->map, kKey2, kKey2Length);

  assert(!removed);
  assert(context->map.count == 1);
  assert(context->map.head_entry == context->map.tail_entry);
  assert(context->map.head_entry->previous == NULL);
  assert(context->map.head_entry->next == NULL);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &Init_Empty_Successful,

    &PutBoolean_False_IsFalse,
    &PutBoolean_True_IsTrue,
    &PutBoolean_CallTwice_Replace,

    &PutEmpty_IsEmpty,
    &PutEmpty_CallTwice_IsEmpty,

    &PutInteger_Zero_IsZero,
    &PutInteger_42_Is42,
    &PutInteger_CallTwice_Replace,

    &PutMap_IsEmptyMap,
    &PutMap_CallTwice_Replace,

    &PutString_Empty_IsEmpty,
    &PutString_Hello_EqualText,
    &PutString_CallTwice_Replace,

    &PutToggle_Enabled_IsEnabled,
    &PutToggle_Disabled_IsDisabled,
    &PutToggle_Key_SetKeyCode,

    &Remove_OneEntryAndExists_Empty,
    &Remove_TwoEntriesAndExists_Removed,
    &Remove_ThreeEntriesAndExists_Removed,
    &Remove_NotExists_DoesNothing
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
