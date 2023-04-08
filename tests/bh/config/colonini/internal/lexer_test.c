/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2022  SlashDiablo Team
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

/**
 * BH
 * Copyright (C) 2011  McGod
 *
 * All rights reserved.
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <shlwapi.h>

#include "bh/config/colonini/internal/lexer.h"

static const char kKeyValueLine[] = "key : value";
static const char kSpacedKeyValueLine[] = "\t     key\v : \tvalue     ";
static const char kNoSpaceKeyValueLine[] = "key:value";
static const char kMappedKeyValueLine[] = "key[innerKey]: value";
static const char kOnlySpacesLine[] = "\t \t\v";
enum {
  kKeyValueLineLength = (sizeof(kKeyValueLine) / sizeof(kKeyValueLine[0])) - 1,
  kSpacedKeyValueLineLength =
      (sizeof(kSpacedKeyValueLine) / sizeof(kSpacedKeyValueLine[0])) - 1,
  kNoSpaceKeyValueLineLength =
      (sizeof(kNoSpaceKeyValueLine) / sizeof(kNoSpaceKeyValueLine[0])) - 1,
  kMappedKeyValueLineLength =
      (sizeof(kMappedKeyValueLine) / sizeof(kMappedKeyValueLine[0])) - 1,
  kOnlySpacesLineLength =
      (sizeof(kOnlySpacesLine) / sizeof(kOnlySpacesLine[0])) - 1
};

static void BeforeAllSetUp(void) {
}

typedef void TestFunc(void);

static void LexLine_KeyValue_ReturnsSuccess(void) {
  struct LexerLine line;
  struct LexerLine* result;

  result = LexerLine_LexLine(&line, 1, kKeyValueLine, kKeyValueLineLength);

  assert(result == &line);

  LexerLine_Deinit(&line);
}

static void LexLine_KeyValue_LexedStrings(void) {
  struct LexerLine line;

  LexerLine_LexLine(&line, 1, kKeyValueLine, kKeyValueLineLength);

  assert(line.strs_count == 5);
  assert(strcmp(line.strs[0].str, "key") == 0);
  assert(line.strs[0].str_length == 3);
  assert(strcmp(line.strs[1].str, " ") == 0);
  assert(line.strs[1].str_length == 1);
  assert(strcmp(line.strs[2].str, ":") == 0);
  assert(line.strs[2].str_length == 1);
  assert(strcmp(line.strs[3].str, " ") == 0);
  assert(line.strs[3].str_length == 1);
  assert(strcmp(line.strs[4].str, "value") == 0);
  assert(line.strs[4].str_length == 5);

  LexerLine_Deinit(&line);
}

static void LexLine_KeyValue_LexedTokens(void) {
  struct LexerLine line;

  LexerLine_LexLine(&line, 1, kKeyValueLine, kKeyValueLineLength);

  assert(line.tokens_count == 3);
  assert(line.first_token == &line.strs[0]);
  assert(line.last_token == &line.strs[4]);
  assert(line.strs[0].previous_token == NULL);
  assert(line.strs[0].next_token == &line.strs[2]);
  assert(line.strs[1].previous_token == &line.strs[0]);
  assert(line.strs[1].next_token == &line.strs[2]);
  assert(line.strs[2].previous_token == &line.strs[0]);
  assert(line.strs[2].next_token == &line.strs[4]);
  assert(line.strs[3].previous_token == &line.strs[2]);
  assert(line.strs[3].next_token == &line.strs[4]);
  assert(line.strs[4].previous_token == &line.strs[2]);
  assert(line.strs[4].next_token == NULL);

  LexerLine_Deinit(&line);
}

static void LexLine_MappedKeyValue_ReturnsSuccess(void) {
  struct LexerLine line;
  struct LexerLine* result;

  result =
      LexerLine_LexLine(
          &line, 1, kMappedKeyValueLine, kMappedKeyValueLineLength);

  assert(result == &line);

  LexerLine_Deinit(&line);
}

static void LexLine_MappedKeyValue_LexedStrings(void) {
  struct LexerLine line;

  LexerLine_LexLine(&line, 1, kMappedKeyValueLine, kMappedKeyValueLineLength);

  assert(line.strs_count == 7);
  assert(strcmp(line.strs[0].str, "key") == 0);
  assert(line.strs[0].str_length == 3);
  assert(strcmp(line.strs[1].str, "[") == 0);
  assert(line.strs[1].str_length == 1);
  assert(strcmp(line.strs[2].str, "innerKey") == 0);
  assert(line.strs[2].str_length == 8);
  assert(strcmp(line.strs[3].str, "]") == 0);
  assert(line.strs[3].str_length == 1);
  assert(strcmp(line.strs[4].str, ":") == 0);
  assert(line.strs[4].str_length == 1);
  assert(strcmp(line.strs[5].str, " ") == 0);
  assert(line.strs[5].str_length == 1);
  assert(strcmp(line.strs[6].str, "value") == 0);
  assert(line.strs[6].str_length == 5);

  LexerLine_Deinit(&line);
}

static void LexLine_MappedKeyValue_LexedTokens(void) {
  struct LexerLine line;

  LexerLine_LexLine(&line, 1, kMappedKeyValueLine, kMappedKeyValueLineLength);

  assert(line.tokens_count == 6);
  assert(line.first_token == &line.strs[0]);
  assert(line.last_token == &line.strs[6]);
  assert(line.strs[0].previous_token == NULL);
  assert(line.strs[0].next_token == &line.strs[1]);
  assert(line.strs[1].previous_token == &line.strs[0]);
  assert(line.strs[1].next_token == &line.strs[2]);
  assert(line.strs[2].previous_token == &line.strs[1]);
  assert(line.strs[2].next_token == &line.strs[3]);
  assert(line.strs[3].previous_token == &line.strs[2]);
  assert(line.strs[3].next_token == &line.strs[4]);
  assert(line.strs[4].previous_token == &line.strs[3]);
  assert(line.strs[4].next_token == &line.strs[6]);
  assert(line.strs[5].previous_token == &line.strs[4]);
  assert(line.strs[5].next_token == &line.strs[6]);
  assert(line.strs[6].previous_token == &line.strs[4]);
  assert(line.strs[6].next_token == NULL);

  LexerLine_Deinit(&line);
}

static void LexLine_SpacedKeyValue_ReturnsSuccess(void) {
  struct LexerLine line;
  struct LexerLine* result;

  result =
      LexerLine_LexLine(
          &line, 1, kSpacedKeyValueLine, kSpacedKeyValueLineLength);

  assert(result == &line);
}

static void LexLine_SpacedKeyValue_LexedStrings(void) {
  struct LexerLine line;

  LexerLine_LexLine(
      &line, 1, kSpacedKeyValueLine, kSpacedKeyValueLineLength);

  assert(line.strs_count == 7);
  assert(strcmp(line.strs[0].str, "\t     ") == 0);
  assert(line.strs[0].str_length == 6);
  assert(strcmp(line.strs[1].str, "key") == 0);
  assert(line.strs[1].str_length == 3);
  assert(strcmp(line.strs[2].str, "\v ") == 0);
  assert(line.strs[2].str_length == 2);
  assert(strcmp(line.strs[3].str, ":") == 0);
  assert(line.strs[3].str_length == 1);
  assert(strcmp(line.strs[4].str, " \t") == 0);
  assert(line.strs[4].str_length == 2);
  assert(strcmp(line.strs[5].str, "value") == 0);
  assert(line.strs[5].str_length == 5);
  assert(strcmp(line.strs[6].str, "     ") == 0);
  assert(line.strs[6].str_length == 5);

  LexerLine_Deinit(&line);
}

static void LexLine_SpacedKeyValue_LexedTokens(void) {
  struct LexerLine line;

  LexerLine_LexLine(
      &line, 1, kSpacedKeyValueLine, kSpacedKeyValueLineLength);

  assert(line.tokens_count == 3);
  assert(line.first_token == &line.strs[1]);
  assert(line.last_token == &line.strs[5]);
  assert(line.strs[0].previous_token == NULL);
  assert(line.strs[0].next_token == &line.strs[1]);
  assert(line.strs[1].previous_token == NULL);
  assert(line.strs[1].next_token == &line.strs[3]);
  assert(line.strs[2].previous_token == &line.strs[1]);
  assert(line.strs[2].next_token == &line.strs[3]);
  assert(line.strs[3].previous_token == &line.strs[1]);
  assert(line.strs[3].next_token == &line.strs[5]);
  assert(line.strs[4].previous_token == &line.strs[3]);
  assert(line.strs[4].next_token == &line.strs[5]);
  assert(line.strs[5].previous_token == &line.strs[3]);
  assert(line.strs[5].next_token == NULL);
  assert(line.strs[6].previous_token == &line.strs[5]);
  assert(line.strs[6].next_token == NULL);

  LexerLine_Deinit(&line);
}

static void LexLine_NoSpaceKeyValue_ReturnsSuccess(void) {
  struct LexerLine line;
  struct LexerLine* result;

  result =
      LexerLine_LexLine(
          &line, 1, kNoSpaceKeyValueLine, kNoSpaceKeyValueLineLength);

  assert(result == &line);

  LexerLine_Deinit(&line);
}

static void LexLine_NoSpaceKeyValue_LexedStrings(void) {
  struct LexerLine line;

  LexerLine_LexLine(
      &line, 1, kNoSpaceKeyValueLine, kNoSpaceKeyValueLineLength);

  assert(line.strs_count == 3);
  assert(strcmp(line.strs[0].str, "key") == 0);
  assert(line.strs[0].str_length == 3);
  assert(strcmp(line.strs[1].str, ":") == 0);
  assert(line.strs[1].str_length == 1);
  assert(strcmp(line.strs[2].str, "value") == 0);
  assert(line.strs[2].str_length == 5);

  LexerLine_Deinit(&line);
}

static void LexLine_NoSpaceKeyValue_LexedTokens(void) {
  struct LexerLine line;

  LexerLine_LexLine(
      &line, 1, kNoSpaceKeyValueLine, kNoSpaceKeyValueLineLength);

  assert(line.tokens_count == 3);
  assert(line.first_token == &line.strs[0]);
  assert(line.last_token == &line.strs[2]);
  assert(line.strs[0].previous_token == NULL);
  assert(line.strs[0].next_token == &line.strs[1]);
  assert(line.strs[1].previous_token == &line.strs[0]);
  assert(line.strs[1].next_token == &line.strs[2]);
  assert(line.strs[2].previous_token == &line.strs[1]);
  assert(line.strs[2].next_token == NULL);

  LexerLine_Deinit(&line);
}

static void LexLine_OnlySpaces_ReturnsSuccess(void) {
  struct LexerLine line;
  struct LexerLine* result;

  result = LexerLine_LexLine(&line, 1, kOnlySpacesLine, kOnlySpacesLineLength);

  assert(result == &line);

  LexerLine_Deinit(&line);
}

static void LexLine_OnlySpaces_OneString(void) {
  struct LexerLine line;

  LexerLine_LexLine(&line, 1, kOnlySpacesLine, kOnlySpacesLineLength);

  assert(line.strs_count == 1);
  assert(strcmp(line.strs[0].str, "\t \t\v") == 0);
  assert(line.strs[0].str_length == 4);

  LexerLine_Deinit(&line);
}

static void LexLine_OnlySpaces_NoTokens(void) {
  struct LexerLine line;

  LexerLine_LexLine(&line, 1, kOnlySpacesLine, kOnlySpacesLineLength);

  assert(line.tokens_count == 0);
  assert(line.first_token == NULL);
  assert(line.last_token == NULL);
  assert(line.strs[0].previous_token == NULL);
  assert(line.strs[0].next_token == NULL);

  LexerLine_Deinit(&line);
}

static void LexLine_Empty_ReturnsSuccess(void) {
  struct LexerLine line;
  struct LexerLine* result;

  result = LexerLine_LexLine(&line, 1, "unused", 0);

  assert(result == &line);

  LexerLine_Deinit(&line);
}

static void LexLine_Empty_NoStrings(void) {
  struct LexerLine line;

  LexerLine_LexLine(&line, 1, "unused", 0);

  assert(line.strs_count == 0);
  assert(line.tokens_count == 0);
  assert(line.first_token == NULL);
  assert(line.last_token == NULL);

  LexerLine_Deinit(&line);
}

static void LexLine_SetLineNumber_ReturnsSuccessWithLineNumber(void) {
  struct LexerLine line;

  LexerLine_LexLine(&line, 42, kKeyValueLine, kKeyValueLineLength);

  assert(line.line_number == 42);

  LexerLine_Deinit(&line);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &LexLine_KeyValue_ReturnsSuccess,
    &LexLine_KeyValue_LexedStrings,
    &LexLine_KeyValue_LexedTokens,

    &LexLine_MappedKeyValue_ReturnsSuccess,
    &LexLine_MappedKeyValue_LexedStrings,
    &LexLine_MappedKeyValue_LexedTokens,

    &LexLine_SpacedKeyValue_ReturnsSuccess,
    &LexLine_SpacedKeyValue_LexedStrings,
    &LexLine_SpacedKeyValue_LexedTokens,

    &LexLine_NoSpaceKeyValue_ReturnsSuccess,
    &LexLine_NoSpaceKeyValue_LexedStrings,
    &LexLine_NoSpaceKeyValue_LexedTokens,

    &LexLine_OnlySpaces_ReturnsSuccess,
    &LexLine_OnlySpaces_OneString,
    &LexLine_OnlySpaces_NoTokens,

    &LexLine_Empty_ReturnsSuccess,
    &LexLine_Empty_NoStrings,

    &LexLine_SetLineNumber_ReturnsSuccessWithLineNumber
  };

  enum {
    kTestsCount = sizeof(kTests) / sizeof(kTests[0])
  };

  size_t i;

  BeforeAllSetUp();

  printf("Running %u test(s).\n", kTestsCount);
  for (i = 0; i < kTestsCount; ++i) {
    kTests[i]();
  }
  printf("Ran %u test(s).\n", kTestsCount);

#endif

  return 0;
}
