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

#include "bh/common/string_util/memstring.h"

typedef void TestFunc(void);

static const char kHelloWorld[] = "Hello World";
static const char kHel[] = "Hel";
static const char kRld[] = "rld";
static const char kWorld[] = "World";
static const char kX[] = "X";

enum {
  kHelloWorldLength = (sizeof(kHelloWorld) / sizeof(kHelloWorld[0])) - 1,
  kHelLength = (sizeof(kHel) / sizeof(kHel[0])) - 1,
  kRldLength = (sizeof(kRld) / sizeof(kRld[0])) - 1,
  kWorldLength = (sizeof(kWorld) / sizeof(kWorld[0])) - 1,
  kXLength = (sizeof(kX) / sizeof(kX[0])) - 1,
};

static void MemCRSpn_HelloWorldOnRld_ReturnsMinus4Index(void) {
  size_t actual;

  actual = MemCRSpn(kHelloWorld, kHelloWorldLength, kRld, kRldLength);

  assert(actual == kHelloWorldLength - 4);
}

static void MemCRSpn_HelloWorldOnX_ReturnsLengthMinus1(void) {
  size_t actual;

  actual = MemCRSpn(kHelloWorld, kHelloWorldLength, kX, kXLength);

  assert(actual == kHelloWorldLength - 1);
}

static void MemCRSpn_HelloWorldOnEmpty_ReturnsLengthMinus1(void) {
  static const char kSearch[] = "";
  enum {
    kSearchLength = (sizeof(kSearch) / sizeof(kSearch[0])) - 1
  };

  size_t actual;

  actual = MemCRSpn(kHelloWorld, kHelloWorldLength, kSearch, kSearchLength);

  assert(actual == kHelloWorldLength - 1);
}

static void MemCRSpn_EmptyStr_ReturnsLength(void) {
  size_t actual;

  actual = MemCRSpn("", 0, kX, kXLength);

  assert(actual == 0);
}

static void MemCRSpn_RldSubstrOnRld_ReturnsSubstrLength(void) {
  size_t actual;

  actual =
      MemCSpn(
          &kHelloWorld[kHelloWorldLength - kRldLength],
          kRldLength,
          kRld,
          kRldLength);

  assert(actual == kRldLength);
}

static void MemCSpn_HelloWorldOnHel_ReturnsOIndex(void) {
  size_t actual;

  actual = MemCSpn(kHelloWorld, kHelloWorldLength, kHel, kHelLength);

  assert(actual == 4);
}

static void MemCSpn_HelloWorldOnX_ReturnsZero(void) {
  size_t actual;

  actual = MemCSpn(kHelloWorld, kHelloWorldLength, kX, kXLength);

  assert(actual == 0);
}

static void MemCSpn_HelloWorldOnEmpty_ReturnsZero(void) {
  static const char kSearch[] = "";
  enum {
    kSearchLength = (sizeof(kSearch) / sizeof(kSearch[0])) - 1
  };

  size_t actual;

  actual = MemCSpn(kHelloWorld, kHelloWorldLength, kSearch, kSearchLength);

  assert(actual == 0);
}

static void MemCSpn_EmptyStr_ReturnsLength(void) {
  size_t actual;

  actual = MemCSpn("", 0, kX, kXLength);

  assert(actual == 0);
}

static void MemCSpn_HelSubstrOnHel_ReturnsSubstrLength(void) {
  size_t actual;

  actual = MemCSpn(kHelloWorld, kHelLength, kHel, kHelLength);

  assert(actual == kHelLength);
}

static void MemRSpn_HelloWorldOnW_ReturnsWIndex(void) {
  static const char kSearch[] = "W";
  enum {
    kSearchLength = (sizeof(kSearch) / sizeof(kSearch[0])) - 1
  };

  size_t actual;

  actual = MemRSpn(kHelloWorld, kHelloWorldLength, kSearch, kSearchLength);

  assert(actual == kHelloWorldLength - 5);
}

static void MemRSpn_HelloWorldOnO_ReturnsLastOIndex(void) {
  static const char kSearch[] = "o";
  enum {
    kSearchLength = (sizeof(kSearch) / sizeof(kSearch[0])) - 1
  };

  size_t actual;

  actual = MemRSpn(kHelloWorld, kHelloWorldLength, kSearch, kSearchLength);

  assert(actual == kHelloWorldLength - 4);
}

static void MemRSpn_HelloWorldOnX_ReturnsLength(void) {
  size_t actual;

  actual = MemRSpn(kHelloWorld, kHelloWorldLength, kX, kXLength);

  assert(actual == kHelloWorldLength);
}

static void MemRSpn_HelloWorldOnEmpty_ReturnsLength(void) {
  static const char kSearch[] = "";
  enum {
    kSearchLength = (sizeof(kSearch) / sizeof(kSearch[0])) - 1
  };

  size_t actual;

  actual = MemRSpn(kHelloWorld, kHelloWorldLength, kSearch, kSearchLength);

  assert(actual == kHelloWorldLength);
}

static void MemRSpn_EmptyStr_ReturnsLength(void) {
  size_t actual;

  actual = MemRSpn("", 0, kX, kXLength);

  assert(actual == 0);
}

static void MemRSpn_RldSubstrOnO_ReturnsSubstrLength(void) {
  static const char kSearch[] = "O";
  enum {
    kSearchLength = (sizeof(kSearch) / sizeof(kSearch[0])) - 1
  };

  size_t actual;

  actual =
      MemRSpn(
          &kHelloWorld[kHelloWorldLength - kRldLength],
          kHelLength,
          kSearch,
          kSearchLength);

  assert(actual == kRldLength);
}

static void MemSpn_HelloWorldOnW_ReturnsWIndex(void) {
  static const char kSearch[] = "W";
  enum {
    kSearchLength = (sizeof(kSearch) / sizeof(kSearch[0])) - 1
  };

  size_t actual;

  actual = MemSpn(kHelloWorld, kHelloWorldLength, kSearch, kSearchLength);

  assert(actual == 6);
}

static void MemSpn_HelloWorldOnO_ReturnsFirstOIndex(void) {
  static const char kSearch[] = "o";
  enum {
    kSearchLength = (sizeof(kSearch) / sizeof(kSearch[0])) - 1
  };

  size_t actual;

  actual = MemSpn(kHelloWorld, kHelloWorldLength, kSearch, kSearchLength);

  assert(actual == 4);
}

static void MemSpn_HelloWorldOnX_ReturnsLength(void) {
  size_t actual;

  actual = MemSpn(kHelloWorld, kHelloWorldLength, kX, kXLength);

  assert(actual == kHelloWorldLength);
}

static void MemSpn_HelloWorldOnEmpty_ReturnsLength(void) {
  static const char kSearch[] = "";
  enum {
    kSearchLength = (sizeof(kSearch) / sizeof(kSearch[0])) - 1
  };

  size_t actual;

  actual = MemSpn(kHelloWorld, kHelloWorldLength, kSearch, kSearchLength);

  assert(actual == kHelloWorldLength);
}

static void MemSpn_EmptyStr_ReturnsLength(void) {
  size_t actual;

  actual = MemSpn("", 0, kX, kXLength);

  assert(actual == 0);
}

static void MemSpn_HelSubstrOnO_ReturnsSubstrLength(void) {
  static const char kSearch[] = "O";
  enum {
    kSearchLength = (sizeof(kSearch) / sizeof(kSearch[0])) - 1
  };

  size_t actual;

  actual = MemSpn(kHelloWorld, kHelLength, kSearch, kSearchLength);

  assert(actual == kHelLength);
}

static void MemStr_HelloWorldOnHel_ReturnsData(void) {
  void* actual;

  actual = MemStr(kHelloWorld, kHelloWorldLength, kHel, kHelLength);

  assert(actual == kHelloWorld);
}

static void MemStr_HelloWorldOnWorld_ReturnsWorldPtr(void) {
  void* actual;

  actual = MemStr(kHelloWorld, kHelloWorldLength, kWorld, kWorldLength);

  assert(actual == &kHelloWorld[6]);
}

static void MemStr_HelloWorldOnX_ReturnsNull(void) {
  void* actual;

  actual = MemStr(kHelloWorld, kHelloWorldLength, kX, kXLength);

  assert(actual == NULL);
}

static void MemStr_HelloSubstrOnWorld_ReturnsNull(void) {
  void* actual;

  actual = MemStr(kHelloWorld, kHelLength, kWorld, kWorldLength);

  assert(actual == NULL);
}

static void MemStr_HelloWorldOnWSubstr_ReturnsWIndex(void) {
  void* actual;

  actual = MemStr(kHelloWorld, kHelloWorldLength, "What", 1);

  assert(actual == &kHelloWorld[6]);
}

static void MemStr_EmptyOnHel_ReturnsNull(void) {
  void* actual;

  actual = MemStr("", 0, kHel, kHelLength);

  assert(actual == NULL);
}

static void MemStr_HelloWorldOnEmpty_ReturnsNull(void) {
  void* actual;

  actual = MemStr(kHelloWorld, kHelloWorldLength, "", 0);

  assert(actual == NULL);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &MemCRSpn_HelloWorldOnRld_ReturnsMinus4Index,
    &MemCRSpn_HelloWorldOnX_ReturnsLengthMinus1,
    &MemCRSpn_HelloWorldOnEmpty_ReturnsLengthMinus1,
    &MemCRSpn_EmptyStr_ReturnsLength,
    &MemCRSpn_RldSubstrOnRld_ReturnsSubstrLength,

    &MemCSpn_HelloWorldOnHel_ReturnsOIndex,
    &MemCSpn_HelloWorldOnX_ReturnsZero,
    &MemCSpn_HelloWorldOnEmpty_ReturnsZero,
    &MemCSpn_EmptyStr_ReturnsLength,
    &MemCSpn_HelSubstrOnHel_ReturnsSubstrLength,

    &MemRSpn_HelloWorldOnW_ReturnsWIndex,
    &MemRSpn_HelloWorldOnO_ReturnsLastOIndex,
    &MemRSpn_HelloWorldOnX_ReturnsLength,
    &MemRSpn_HelloWorldOnEmpty_ReturnsLength,
    &MemRSpn_EmptyStr_ReturnsLength,
    &MemRSpn_RldSubstrOnO_ReturnsSubstrLength,

    &MemSpn_HelloWorldOnW_ReturnsWIndex,
    &MemSpn_HelloWorldOnO_ReturnsFirstOIndex,
    &MemSpn_HelloWorldOnX_ReturnsLength,
    &MemSpn_HelloWorldOnEmpty_ReturnsLength,
    &MemSpn_EmptyStr_ReturnsLength,
    &MemSpn_HelSubstrOnO_ReturnsSubstrLength,

    &MemStr_HelloWorldOnHel_ReturnsData,
    &MemStr_HelloWorldOnWorld_ReturnsWorldPtr,
    &MemStr_HelloWorldOnX_ReturnsNull,
    &MemStr_HelloSubstrOnWorld_ReturnsNull,
    &MemStr_HelloWorldOnWSubstr_ReturnsWIndex,
    &MemStr_EmptyOnHel_ReturnsNull,
    &MemStr_HelloWorldOnEmpty_ReturnsNull
  };

  enum {
    kTestsCount = sizeof(kTests) / sizeof(kTests[0])
  };

  size_t i;

  printf("Running %u test(s).\n", kTestsCount);
  for (i = 0; i < kTestsCount; ++i) {
    kTests[i]();
  }
  printf("Ran %u test(s).\n", kTestsCount);

#endif

  return 0;
}
