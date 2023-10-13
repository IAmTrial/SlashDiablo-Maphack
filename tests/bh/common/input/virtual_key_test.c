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
#include <wchar.h>
#include <windows.h>

#include "bh/common/input/virtual_key.h"

static const char kVkPriorKeyStr[] = "Vk_PrIoR";
static const char kVkPageUpKeyStr[] = "vK_pAgEuP";

static const char kVkPriorStr[] = "VK_PRIOR";
static const char kVkPageUpStr[] = "VK_PAGEUP";

enum {
  kVkPriorStrLength = sizeof(kVkPriorStr) / sizeof(kVkPriorStr[0]) - 1,
  kVkPageUpStrLength = sizeof(kVkPageUpStr) / sizeof(kVkPageUpStr[0]) - 1
};

static void BeforeAllSetUp(void) {}

struct EachContext {
  unsigned char unused;
};

typedef void TestFunc(struct EachContext* context);

static void BeforeEach(struct EachContext* context) {}

static void AfterEach(struct EachContext* context) {}

static void GetFromCode_Prior_GetsPrior(struct EachContext* context) {
  const struct Input_VirtualKey* result;

  result = Input_VirtualKey_GetFromCode(VK_PRIOR);

  assert(result->code == VK_PRIOR);
  assert(strcmp(result->symbol_name, kVkPriorStr) == 0);
  assert(strcmp(result->old_symbol_name, kVkPageUpStr) == 0);
}

static void GetFromSymbolName_Prior_GetsPrior(struct EachContext* context) {
  const struct Input_VirtualKey* result;

  result = Input_VirtualKey_GetFromSymbolName(kVkPriorKeyStr, kVkPriorStrLength);

  assert(result->code == VK_PRIOR);
  assert(strcmp(result->symbol_name, kVkPriorStr) == 0);
  assert(strcmp(result->old_symbol_name, kVkPageUpStr) == 0);
}

static void GetFromSymbolName_PageUp_GetsPrior(struct EachContext* context) {
  const struct Input_VirtualKey* result;

  result =
      Input_VirtualKey_GetFromSymbolName(kVkPageUpKeyStr, kVkPageUpStrLength);

  assert(result->code == VK_PRIOR);
  assert(strcmp(result->symbol_name, kVkPriorStr) == 0);
  assert(strcmp(result->old_symbol_name, kVkPageUpStr) == 0);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &GetFromCode_Prior_GetsPrior,

    &GetFromSymbolName_Prior_GetsPrior,
    &GetFromSymbolName_PageUp_GetsPrior
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
