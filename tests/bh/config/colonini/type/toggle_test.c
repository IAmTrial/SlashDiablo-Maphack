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

#include "bh/config/colonini/type/toggle.h"

static void BeforeAllSetUp(void) {}

struct EachContext {
  unsigned char unused;
};

typedef void TestFunc(struct EachContext* context);

static void BeforeEach(struct EachContext* context) {}

static void AfterEach(struct EachContext* context) {}

static void Init_Enabled_IsEnabled(struct EachContext* context) {
  struct Colonini_Toggle toggle;
  struct Colonini_Toggle* toggle_init_result;

  toggle_init_result = Colonini_Toggle_Init(&toggle, 1, 0);

  assert(toggle_init_result != NULL);
  assert(toggle.enabled);

  Colonini_Toggle_Deinit(&toggle);
}

static void Init_Disabled_IsDisabled(struct EachContext* context) {
  struct Colonini_Toggle toggle;
  struct Colonini_Toggle* toggle_init_result;

  toggle_init_result = Colonini_Toggle_Init(&toggle, 0, 0);

  assert(toggle_init_result != NULL);
  assert(!toggle.enabled);

  Colonini_Toggle_Deinit(&toggle);
}

static void Init_Key_SetKeyCode(struct EachContext* context) {
  struct Colonini_Toggle toggle;
  struct Colonini_Toggle* toggle_init_result;

  toggle_init_result = Colonini_Toggle_Init(&toggle, 0, 42);

  assert(toggle_init_result != NULL);
  assert(toggle.key_code == 42);

  Colonini_Toggle_Deinit(&toggle);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &Init_Enabled_IsEnabled,
    &Init_Disabled_IsDisabled,
    &Init_Key_SetKeyCode
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
