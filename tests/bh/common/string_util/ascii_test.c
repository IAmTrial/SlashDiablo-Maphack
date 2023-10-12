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
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <wchar.h>

#include "bh/common/string_util/ascii.h"

static const char kNonAlphaChars[] =
    "\x0\x1\x2\x3\x4\x5\x6\a\b\t\n\v\f\r\xE\xF\x10\x11\x12\x13\x14\x15"
        "\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F !\"#$%&\'()*+,-./"
        "0123456789:;<=>\?@[\\]^_`{|}~\x7F";
static const char kLowercaseChars[] = "abcdefghijklmnopqrstuvwxyz";
static const char kUppercaseChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static const wchar_t kNonAlphaWChars[] =
    L"\x0\x1\x2\x3\x4\x5\x6\a\b\t\n\v\f\r\xE\xF\x10\x11\x12\x13\x14\x15"
        L"\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F !\"#$%&\'()*+,-./"
        L"0123456789:;<=>\?@[\\]^_`{|}~\x7F";
static const wchar_t kLowercaseWChars[] = L"abcdefghijklmnopqrstuvwxyz";
static const wchar_t kUppercaseWChars[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static const char kAsciiStr[] =
    "\x0\x1\x2\x3\x4\x5\x6\a\b\t\n\v\f\r\xE\xF\x10\x11\x12\x13\x14\x15"
        "\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F !\"#$%&\'()*+,-./"
        "0123456789:;<=>\?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
        "abcdefghijklmnopqrstuvwxyz{|}~\x7F";
static const char kLowercaseStr[] =
    "\x0\x1\x2\x3\x4\x5\x6\a\b\t\n\v\f\r\xE\xF\x10\x11\x12\x13\x14\x15"
        "\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F !\"#$%&\'()*+,-./"
        "0123456789:;<=>\?@abcdefghijklmnopqrstuvwxyz[\\]^_`"
        "abcdefghijklmnopqrstuvwxyz{|}~\x7F";
static const char kUppercaseStr[] =
    "\x0\x1\x2\x3\x4\x5\x6\a\b\t\n\v\f\r\xE\xF\x10\x11\x12\x13\x14\x15"
        "\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F !\"#$%&\'()*+,-./"
        "0123456789:;<=>\?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ{|}~\x7F";

static const wchar_t kAsciiWStr[] =
    L"\x0\x1\x2\x3\x4\x5\x6\a\b\t\n\v\f\r\xE\xF\x10\x11\x12\x13\x14\x15"
        L"\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F !\"#$%&\'()*+,-./"
        L"0123456789:;<=>\?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
        L"abcdefghijklmnopqrstuvwxyz{|}~\x7F";
static const wchar_t kLowercaseWStr[] =
    L"\x0\x1\x2\x3\x4\x5\x6\a\b\t\n\v\f\r\xE\xF\x10\x11\x12\x13\x14\x15"
        L"\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F !\"#$%&\'()*+,-./"
        L"0123456789:;<=>\?@abcdefghijklmnopqrstuvwxyz[\\]^_`"
        L"abcdefghijklmnopqrstuvwxyz{|}~\x7F";
static const wchar_t kUppercaseWStr[] =
    L"\x0\x1\x2\x3\x4\x5\x6\a\b\t\n\v\f\r\xE\xF\x10\x11\x12\x13\x14\x15"
        L"\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F !\"#$%&\'()*+,-./"
        L"0123456789:;<=>\?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
        L"ABCDEFGHIJKLMNOPQRSTUVWXYZ{|}~\x7F";

enum {
  kAlphaCharsLength =
      sizeof(kLowercaseChars) / sizeof(kLowercaseChars[0]) - 1,
  kNonAlphaCharsLength =
      sizeof(kNonAlphaChars) / sizeof(kNonAlphaChars[0]) - 1,

  kAlphaWCharsLength =
      sizeof(kLowercaseWChars) / sizeof(kLowercaseWChars[0]) - 1,
  kNonAlphaWCharsLength =
      sizeof(kNonAlphaWChars) / sizeof(kNonAlphaWChars[0]) - 1,

  kAsciiStrLength = sizeof(kAsciiStr) / sizeof(kAsciiStr[0]) - 1,

  kAsciiWStrLength = sizeof(kAsciiWStr) / sizeof(kAsciiWStr[0]) - 1,
};

static void BeforeAllSetUp(void) {}

struct EachContext {
  unsigned char unused;
};

typedef void TestFunc(struct EachContext* context);

static void BeforeEach(struct EachContext* context) {}

static void AfterEach(struct EachContext* context) {}

static void ToLowerChar_Uppercase_Lowercase(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kAlphaCharsLength; ++i) {
    char ch;

    ch = Ascii_ToLowerChar(kUppercaseChars[i]);

    assert(ch == kLowercaseChars[i]);
  }
}

static void ToLowerChar_Lowercase_Same(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kAlphaCharsLength; ++i) {
    char ch;

    ch = Ascii_ToLowerChar(kLowercaseChars[i]);

    assert(ch == kLowercaseChars[i]);
  }
}

static void ToLowerChar_NonAlpha_Same(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kNonAlphaCharsLength; ++i) {
    char ch;

    ch = Ascii_ToLowerChar(kNonAlphaChars[i]);

    assert(ch == kNonAlphaChars[i]);
  }
}

static void ToLowerWChar_Uppercase_Lowercase(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kAlphaWCharsLength; ++i) {
    wchar_t ch;

    ch = Ascii_ToLowerWChar(kUppercaseWChars[i]);

    assert(ch == kLowercaseWChars[i]);
  }
}

static void ToLowerWChar_Lowercase_Same(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kAlphaWCharsLength; ++i) {
    wchar_t ch;

    ch = Ascii_ToLowerWChar(kLowercaseWChars[i]);

    assert(ch == kLowercaseWChars[i]);
  }
}

static void ToLowerWChar_NonAlpha_Same(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kNonAlphaWCharsLength; ++i) {
    wchar_t ch;

    ch = Ascii_ToLowerWChar(kNonAlphaWChars[i]);

    assert(ch == kNonAlphaWChars[i]);
  }
}

static void ToLowerStr_AsciiStr_Lowercase(struct EachContext* context) {
  char lower_alpha[kAsciiStrLength + 1];

  Ascii_ToLowerStr(lower_alpha, kAsciiStr, kAsciiStrLength);

  assert(memcmp(lower_alpha, kLowercaseStr, kAsciiStrLength + 1) == 0);
}

static void ToLowerWStr_AsciiStr_Lowercase(struct EachContext* context) {
  wchar_t lower_alpha[kAsciiStrLength + 1];

  Ascii_ToLowerWStr(lower_alpha, kAsciiWStr, kAsciiWStrLength);

  assert(wmemcmp(lower_alpha, kLowercaseWStr, kAsciiWStrLength + 1) == 0);
}

static void ToUpperChar_Lowercase_Uppercase(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kAlphaCharsLength; ++i) {
    char ch;

    ch = Ascii_ToUpperChar(kLowercaseChars[i]);

    assert(ch == kUppercaseChars[i]);
  }
}

static void ToUpperChar_Uppercase_Same(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kAlphaCharsLength; ++i) {
    char ch;

    ch = Ascii_ToUpperChar(kUppercaseChars[i]);

    assert(ch == kUppercaseChars[i]);
  }
}

static void ToUpperChar_NonAlpha_Same(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kNonAlphaCharsLength; ++i) {
    char ch;

    ch = Ascii_ToUpperChar(kNonAlphaChars[i]);

    assert(ch == kNonAlphaChars[i]);
  }
}

static void ToUpperWChar_Lowercase_Uppercase(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kAlphaWCharsLength; ++i) {
    wchar_t ch;

    ch = Ascii_ToUpperWChar(kLowercaseWChars[i]);

    assert(ch == kUppercaseWChars[i]);
  }
}

static void ToUpperWChar_Uppercase_Same(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kAlphaWCharsLength; ++i) {
    wchar_t ch;

    ch = Ascii_ToUpperWChar(kUppercaseWChars[i]);

    assert(ch == kUppercaseWChars[i]);
  }
}

static void ToUpperWChar_NonAlpha_Same(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kNonAlphaWCharsLength; ++i) {
    wchar_t ch;

    ch = Ascii_ToUpperWChar(kNonAlphaWChars[i]);

    assert(ch == kNonAlphaWChars[i]);
  }
}

static void ToUpperStr_AsciiStr_Uppercase(struct EachContext* context) {
  char upper_alpha[kAsciiStrLength + 1];

  Ascii_ToUpperStr(upper_alpha, kAsciiStr, kAsciiStrLength);

  assert(memcmp(upper_alpha, kUppercaseStr, kAsciiStrLength + 1) == 0);
}

static void ToUpperWStr_AsciiStr_Uppercase(struct EachContext* context) {
  wchar_t upper_alpha[kAsciiWStrLength + 1];

  Ascii_ToUpperWStr(upper_alpha, kAsciiWStr, kAsciiWStrLength);

  assert(wmemcmp(upper_alpha, kUppercaseWStr, kAsciiWStrLength + 1) == 0);
}

static void TrimWhitespaceChars_NoPrefixNoSuffix_Same(
    struct EachContext* context) {
  static const char kSource[] = "No Prefix\tNo\vSuffix";
  static const char kExpected[] = "No Prefix\tNo\vSuffix";
  enum {
    kSourceLength = sizeof(kSource) / sizeof(kSource[0]) - 1,
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  char dest[64];

  Ascii_TrimWhitespaceChars(dest, kSource, kSourceLength);

  assert(memcmp(dest, kExpected, kExpectedLength) == 0);
}

static void TrimWhitespaceChars_WithPrefix_TrimPrefix(
    struct EachContext* context) {
  static const char kSource[] = " \v  \tWith Prefix";
  static const char kExpected[] = "With Prefix";
  enum {
    kSourceLength = sizeof(kSource) / sizeof(kSource[0]) - 1,
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  char dest[64];

  Ascii_TrimWhitespaceChars(dest, kSource, kSourceLength);

  assert(memcmp(dest, kExpected, kExpectedLength) == 0);
}

static void TrimWhitespaceChars_WithSuffix_TrimSuffix(
    struct EachContext* context) {
  static const char kSource[] = "With Suffix\t \v   ";
  static const char kExpected[] = "With Suffix";
  enum {
    kSourceLength = sizeof(kSource) / sizeof(kSource[0]) - 1,
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  char dest[64];

  Ascii_TrimWhitespaceChars(dest, kSource, kSourceLength);

  assert(memcmp(dest, kExpected, kExpectedLength) == 0);
}

static void TrimWhitespaceChars_WithPrefixAndSuffix_TrimSuffix(
    struct EachContext* context) {
  static const char kSource[] = "\v   \t\rWith Prefix\tAnd\vSuffix\t   \n";
  static const char kExpected[] = "With Prefix\tAnd\vSuffix";
  enum {
    kSourceLength = sizeof(kSource) / sizeof(kSource[0]) - 1,
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  char dest[64];

  Ascii_TrimWhitespaceChars(dest, kSource, kSourceLength);

  assert(memcmp(dest, kExpected, kExpectedLength) == 0);
}

static void TrimWhitespaceWChars_NoPrefixNoSuffix_Same(
    struct EachContext* context) {
  static const wchar_t kSource[] = L"No Prefix\tNo\vSuffix";
  static const wchar_t kExpected[] = L"No Prefix\tNo\vSuffix";
  enum {
    kSourceLength = sizeof(kSource) / sizeof(kSource[0]) - 1,
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  wchar_t dest[64];

  Ascii_TrimWhitespaceWChars(dest, kSource, kSourceLength);

  assert(wmemcmp(dest, kExpected, kExpectedLength) == 0);
}

static void TrimWhitespaceWChars_WithPrefix_TrimPrefix(
    struct EachContext* context) {
  static const wchar_t kSource[] = L" \v  \tWith Prefix";
  static const wchar_t kExpected[] = L"With Prefix";
  enum {
    kSourceLength = sizeof(kSource) / sizeof(kSource[0]) - 1,
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  wchar_t dest[64];

  Ascii_TrimWhitespaceWChars(dest, kSource, kSourceLength);

  assert(wmemcmp(dest, kExpected, kExpectedLength) == 0);
}

static void TrimWhitespaceWChars_WithSuffix_TrimSuffix(
    struct EachContext* context) {
  static const wchar_t kSource[] = L"With Suffix\t \v   ";
  static const wchar_t kExpected[] = L"With Suffix";
  enum {
    kSourceLength = sizeof(kSource) / sizeof(kSource[0]) - 1,
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  wchar_t dest[64];

  Ascii_TrimWhitespaceWChars(dest, kSource, kSourceLength);

  assert(wmemcmp(dest, kExpected, kExpectedLength) == 0);
}

static void TrimWhitespaceWChars_WithPrefixAndSuffix_TrimSuffix(
    struct EachContext* context) {
  static const wchar_t kSource[] = L"\v   \t\rWith Prefix\tAnd\vSuffix\t   \n";
  static const wchar_t kExpected[] = L"With Prefix\tAnd\vSuffix";
  enum {
    kSourceLength = sizeof(kSource) / sizeof(kSource[0]) - 1,
    kExpectedLength = sizeof(kExpected) / sizeof(kExpected[0]) - 1
  };

  wchar_t dest[64];

  Ascii_TrimWhitespaceWChars(dest, kSource, kSourceLength);

  assert(wmemcmp(dest, kExpected, kExpectedLength) == 0);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &ToLowerChar_Uppercase_Lowercase,
    &ToLowerChar_Lowercase_Same,
    &ToLowerChar_NonAlpha_Same,
    &ToLowerWChar_Uppercase_Lowercase,
    &ToLowerWChar_Lowercase_Same,
    &ToLowerWChar_NonAlpha_Same,

    &ToLowerStr_AsciiStr_Lowercase,
    &ToLowerWStr_AsciiStr_Lowercase,

    &ToUpperChar_Lowercase_Uppercase,
    &ToUpperChar_Uppercase_Same,
    &ToUpperChar_NonAlpha_Same,
    &ToUpperWChar_Lowercase_Uppercase,
    &ToUpperWChar_Uppercase_Same,
    &ToUpperWChar_NonAlpha_Same,

    &ToUpperStr_AsciiStr_Uppercase,
    &ToUpperWStr_AsciiStr_Uppercase,

    &TrimWhitespaceChars_NoPrefixNoSuffix_Same,
    &TrimWhitespaceChars_WithPrefix_TrimPrefix,
    &TrimWhitespaceChars_WithSuffix_TrimSuffix,
    &TrimWhitespaceChars_WithPrefixAndSuffix_TrimSuffix,
    &TrimWhitespaceWChars_NoPrefixNoSuffix_Same,
    &TrimWhitespaceWChars_WithPrefix_TrimPrefix,
    &TrimWhitespaceWChars_WithSuffix_TrimSuffix,
    &TrimWhitespaceWChars_WithPrefixAndSuffix_TrimSuffix
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
