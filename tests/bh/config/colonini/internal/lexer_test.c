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

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <shlwapi.h>

#include "bh/config/colonini/internal/lexer.h"
#include "bh/config/colonini/internal/lexer/lexer_line.h"
#include "bh/config/colonini/type/string.h"

static char* const kKeyValueLine = "key : value";
static struct Colonini_String kKeyValueLineStr;
static char* const kSpacedKeyValueLine = "\t     key\v : \tvalue     ";
static struct Colonini_String kSpacedKeyValueLineStr;
static char* const kNoSpaceKeyValueLine = "key:value";
static struct Colonini_String kNoSpaceKeyValueLineStr;
static char* const kMappedKeyValueLine = "key[innerKey]: value, true";
static struct Colonini_String kMappedKeyValueLineStr;
static char* const kOnlySpacesLine = "\t \t\v";
static struct Colonini_String kOnlySpacesLineStr;
static char* const kEmptyLine = "";
static struct Colonini_String kEmptyLineStr;
static char* const kCommentLine = "// key : value";
static struct Colonini_String kCommentLineStr;

static void BeforeAllSetUp(void) {
  Colonini_String_Init(
      &kKeyValueLineStr, kKeyValueLine, strlen(kKeyValueLine));
  Colonini_String_Init(
      &kSpacedKeyValueLineStr,
      kSpacedKeyValueLine,
      strlen(kSpacedKeyValueLine));
  Colonini_String_Init(
      &kNoSpaceKeyValueLineStr,
      kNoSpaceKeyValueLine,
      strlen(kNoSpaceKeyValueLine));
  Colonini_String_Init(
      &kMappedKeyValueLineStr,
      kMappedKeyValueLine,
      strlen(kMappedKeyValueLine));
  Colonini_String_Init(
      &kOnlySpacesLineStr, kOnlySpacesLine, strlen(kOnlySpacesLine));
  Colonini_String_Init(&kEmptyLineStr, kEmptyLine, strlen(kEmptyLine));
  Colonini_String_Init(&kCommentLineStr, kCommentLine, strlen(kCommentLine));
}

struct EachContext {
  struct Lexer lexer;
};

typedef void TestFunc(struct EachContext* context);

static void BeforeEach(struct EachContext* context) {
  struct Lexer* result;

  result = Lexer_Init(&context->lexer, 16);
  assert(result == &context->lexer);
}

static void AfterEach(struct EachContext* context) {
  Lexer_Deinit(&context->lexer);
}

static void Lex_KeyValue_ReturnsSuccess(struct EachContext* context) {
  int result;

  result = Lexer_Lex(&context->lexer, &kKeyValueLineStr, 1);

  assert(result);
}

static void Lex_KeyValue_LexedStrings(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kKeyValueLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->str_count == 5);
  assert(strcmp(lline->strs[0].str.str, "key") == 0);
  assert(lline->strs[0].str.length == 3);
  assert(lline->strs[0].line_index == 0);
  assert(strcmp(lline->strs[1].str.str, " ") == 0);
  assert(lline->strs[1].str.length == 1);
  assert(lline->strs[1].line_index == 3);
  assert(strcmp(lline->strs[2].str.str, ":") == 0);
  assert(lline->strs[2].str.length == 1);
  assert(lline->strs[2].line_index == 4);
  assert(strcmp(lline->strs[3].str.str, " ") == 0);
  assert(lline->strs[3].str.length == 1);
  assert(lline->strs[3].line_index == 5);
  assert(strcmp(lline->strs[4].str.str, "value") == 0);
  assert(lline->strs[4].str.length == 5);
  assert(lline->strs[4].line_index == 6);
}

static void Lex_KeyValue_LexedTokens(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kKeyValueLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->token_count == 3);
  assert(lline->first_token == &lline->strs[0]);
  assert(lline->last_token == &lline->strs[4]);
  assert(lline->strs[0].previous_token == NULL);
  assert(lline->strs[0].next_token == &lline->strs[2]);
  assert(lline->strs[1].previous_token == &lline->strs[0]);
  assert(lline->strs[1].next_token == &lline->strs[2]);
  assert(lline->strs[2].previous_token == &lline->strs[0]);
  assert(lline->strs[2].next_token == &lline->strs[4]);
  assert(lline->strs[3].previous_token == &lline->strs[2]);
  assert(lline->strs[3].next_token == &lline->strs[4]);
  assert(lline->strs[4].previous_token == &lline->strs[2]);
  assert(lline->strs[4].next_token == NULL);
}

static void Lex_MappedKeyValue_ReturnsSuccess(struct EachContext* context) {
  int result;

  result = Lexer_Lex(&context->lexer, &kMappedKeyValueLineStr, 1);

  assert(result);
}

static void Lex_MappedKeyValue_LexedStrings(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kMappedKeyValueLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->str_count == 10);
  assert(strcmp(lline->strs[0].str.str, "key") == 0);
  assert(lline->strs[0].str.length == 3);
  assert(lline->strs[0].line_index == 0);
  assert(strcmp(lline->strs[1].str.str, "[") == 0);
  assert(lline->strs[1].str.length == 1);
  assert(lline->strs[1].line_index == 3);
  assert(strcmp(lline->strs[2].str.str, "innerKey") == 0);
  assert(lline->strs[2].str.length == 8);
  assert(lline->strs[2].line_index == 4);
  assert(strcmp(lline->strs[3].str.str, "]") == 0);
  assert(lline->strs[3].str.length == 1);
  assert(lline->strs[3].line_index == 12);
  assert(strcmp(lline->strs[4].str.str, ":") == 0);
  assert(lline->strs[4].str.length == 1);
  assert(lline->strs[4].line_index == 13);
  assert(strcmp(lline->strs[5].str.str, " ") == 0);
  assert(lline->strs[5].str.length == 1);
  assert(lline->strs[5].line_index == 14);
  assert(strcmp(lline->strs[6].str.str, "value") == 0);
  assert(lline->strs[6].str.length == 5);
  assert(lline->strs[6].line_index == 15);
  assert(strcmp(lline->strs[7].str.str, ",") == 0);
  assert(lline->strs[7].str.length == 1);
  assert(lline->strs[7].line_index == 20);
  assert(strcmp(lline->strs[8].str.str, " ") == 0);
  assert(lline->strs[8].str.length == 1);
  assert(lline->strs[8].line_index == 21);
  assert(strcmp(lline->strs[9].str.str, "true") == 0);
  assert(lline->strs[9].str.length == 4);
  assert(lline->strs[9].line_index == 22);
}

static void Lex_MappedKeyValue_LexedTokens(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kMappedKeyValueLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->token_count == 8);
  assert(lline->first_token == &lline->strs[0]);
  assert(lline->last_token == &lline->strs[9]);
  assert(lline->strs[0].previous_token == NULL);
  assert(lline->strs[0].next_token == &lline->strs[1]);
  assert(lline->strs[1].previous_token == &lline->strs[0]);
  assert(lline->strs[1].next_token == &lline->strs[2]);
  assert(lline->strs[2].previous_token == &lline->strs[1]);
  assert(lline->strs[2].next_token == &lline->strs[3]);
  assert(lline->strs[3].previous_token == &lline->strs[2]);
  assert(lline->strs[3].next_token == &lline->strs[4]);
  assert(lline->strs[4].previous_token == &lline->strs[3]);
  assert(lline->strs[4].next_token == &lline->strs[6]);
  assert(lline->strs[5].previous_token == &lline->strs[4]);
  assert(lline->strs[5].next_token == &lline->strs[6]);
  assert(lline->strs[6].previous_token == &lline->strs[4]);
  assert(lline->strs[6].next_token == &lline->strs[7]);
  assert(lline->strs[7].previous_token == &lline->strs[6]);
  assert(lline->strs[7].next_token == &lline->strs[9]);
  assert(lline->strs[8].previous_token == &lline->strs[7]);
  assert(lline->strs[8].next_token == &lline->strs[9]);
  assert(lline->strs[9].previous_token == &lline->strs[7]);
  assert(lline->strs[9].next_token == NULL);
}

static void Lex_SpacedKeyValue_ReturnsSuccess(struct EachContext* context) {
  int result;

  result = Lexer_Lex(&context->lexer, &kSpacedKeyValueLineStr, 1);

  assert(result);
}

static void Lex_SpacedKeyValue_LexedStrings(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kSpacedKeyValueLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->str_count == 7);
  assert(strcmp(lline->strs[0].str.str, "\t     ") == 0);
  assert(lline->strs[0].str.length == 6);
  assert(lline->strs[0].line_index == 0);
  assert(strcmp(lline->strs[1].str.str, "key") == 0);
  assert(lline->strs[1].str.length == 3);
  assert(lline->strs[1].line_index == 6);
  assert(strcmp(lline->strs[2].str.str, "\v ") == 0);
  assert(lline->strs[2].str.length == 2);
  assert(lline->strs[2].line_index == 9);
  assert(strcmp(lline->strs[3].str.str, ":") == 0);
  assert(lline->strs[3].str.length == 1);
  assert(lline->strs[3].line_index == 11);
  assert(strcmp(lline->strs[4].str.str, " \t") == 0);
  assert(lline->strs[4].str.length == 2);
  assert(lline->strs[4].line_index == 12);
  assert(strcmp(lline->strs[5].str.str, "value") == 0);
  assert(lline->strs[5].str.length == 5);
  assert(lline->strs[5].line_index == 14);
  assert(strcmp(lline->strs[6].str.str, "     ") == 0);
  assert(lline->strs[6].str.length == 5);
  assert(lline->strs[6].line_index == 19);
}

static void Lex_SpacedKeyValue_LexedTokens(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kSpacedKeyValueLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->token_count == 3);
  assert(lline->first_token == &lline->strs[1]);
  assert(lline->last_token == &lline->strs[5]);
  assert(lline->strs[0].previous_token == NULL);
  assert(lline->strs[0].next_token == &lline->strs[1]);
  assert(lline->strs[1].previous_token == NULL);
  assert(lline->strs[1].next_token == &lline->strs[3]);
  assert(lline->strs[2].previous_token == &lline->strs[1]);
  assert(lline->strs[2].next_token == &lline->strs[3]);
  assert(lline->strs[3].previous_token == &lline->strs[1]);
  assert(lline->strs[3].next_token == &lline->strs[5]);
  assert(lline->strs[4].previous_token == &lline->strs[3]);
  assert(lline->strs[4].next_token == &lline->strs[5]);
  assert(lline->strs[5].previous_token == &lline->strs[3]);
  assert(lline->strs[5].next_token == NULL);
  assert(lline->strs[6].previous_token == &lline->strs[5]);
  assert(lline->strs[6].next_token == NULL);
}

static void Lex_NoSpaceKeyValue_ReturnsSuccess(struct EachContext* context) {
  int result;

  result = Lexer_Lex(&context->lexer, &kNoSpaceKeyValueLineStr, 1);

  assert(result);
}

static void Lex_NoSpaceKeyValue_LexedStrings(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kNoSpaceKeyValueLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->str_count == 3);
  assert(strcmp(lline->strs[0].str.str, "key") == 0);
  assert(lline->strs[0].str.length == 3);
  assert(lline->strs[0].line_index == 0);
  assert(strcmp(lline->strs[1].str.str, ":") == 0);
  assert(lline->strs[1].str.length == 1);
  assert(lline->strs[1].line_index == 3);
  assert(strcmp(lline->strs[2].str.str, "value") == 0);
  assert(lline->strs[2].str.length == 5);
  assert(lline->strs[2].line_index == 4);
}

static void Lex_NoSpaceKeyValue_LexedTokens(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kNoSpaceKeyValueLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->token_count == 3);
  assert(lline->first_token == &lline->strs[0]);
  assert(lline->last_token == &lline->strs[2]);
  assert(lline->strs[0].previous_token == NULL);
  assert(lline->strs[0].next_token == &lline->strs[1]);
  assert(lline->strs[1].previous_token == &lline->strs[0]);
  assert(lline->strs[1].next_token == &lline->strs[2]);
  assert(lline->strs[2].previous_token == &lline->strs[1]);
  assert(lline->strs[2].next_token == NULL);
}

static void Lex_OnlySpaces_ReturnsSuccess(struct EachContext* context) {
  int result;

  result = Lexer_Lex(&context->lexer, &kOnlySpacesLineStr, 1);

  assert(result);
}

static void Lex_OnlySpaces_OneString(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kOnlySpacesLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->str_count == 1);
  assert(strcmp(lline->strs[0].str.str, "\t \t\v") == 0);
  assert(lline->strs[0].str.length == 4);
  assert(lline->strs[0].line_index == 0);
}

static void Lex_OnlySpaces_NoTokens(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kOnlySpacesLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->token_count == 0);
  assert(lline->first_token == NULL);
  assert(lline->last_token == NULL);
  assert(lline->strs[0].previous_token == NULL);
  assert(lline->strs[0].next_token == NULL);
}

static void Lex_Empty_ReturnsSuccess(struct EachContext* context) {
  int result;

  result = Lexer_Lex(&context->lexer, &kEmptyLineStr, 1);

  assert(result);
}

static void Lex_Empty_NoStrings(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kEmptyLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->str_count == 0);
  assert(lline->token_count == 0);
  assert(lline->first_token == NULL);
  assert(lline->last_token == NULL);
}

static void Lex_MultipleEmpty_NoStrings(struct EachContext* context) {
  enum {
    kLines = 10
  };
  size_t i;
  struct Colonini_String kEmptyLineStrs[kLines];
  for (i = 0; i < kLines; ++i) {
    Colonini_String_Init(&kEmptyLineStrs[i], "", 0);
  }

  Lexer_Lex(&context->lexer, kEmptyLineStrs, kLines);

  assert(context->lexer.count == kLines);
  for (i = 0; i < kLines; ++i) {
    struct LexerLine* lline;
    lline = &context->lexer.lines[i];
    assert(lline->str_count == 0);
    assert(lline->token_count == 0);
    assert(lline->first_token == NULL);
    assert(lline->last_token == NULL);
  }
}

static void Lex_CommentLine_ReturnsSuccess(struct EachContext* context) {
  int result;

  result = Lexer_Lex(&context->lexer, &kCommentLineStr, 1);

  assert(result);
}

static void Lex_CommentLine_OneString(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kCommentLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->str_count == 1);
  assert(strcmp(lline->strs[0].str.str, kCommentLine) == 0);
  assert(lline->strs[0].str.length == strlen(kCommentLine));
  assert(lline->strs[0].line_index == 0);
}

static void Lex_CommentLine_NoTokens(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kCommentLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->token_count == 0);
  assert(lline->first_token == NULL);
  assert(lline->last_token == NULL);
  assert(lline->strs[0].previous_token == NULL);
  assert(lline->strs[0].next_token == NULL);
}

static void Lex_SetLineNumber_ReturnsSuccessWithLineNumber(struct EachContext* context) {
  struct LexerLine* lline;

  Lexer_Lex(&context->lexer, &kKeyValueLineStr, 1);

  lline = &context->lexer.lines[0];
  assert(lline->line_number == 1);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &Lex_KeyValue_ReturnsSuccess,
    &Lex_KeyValue_LexedStrings,
    &Lex_KeyValue_LexedTokens,

    &Lex_MappedKeyValue_ReturnsSuccess,
    &Lex_MappedKeyValue_LexedStrings,
    &Lex_MappedKeyValue_LexedTokens,

    &Lex_SpacedKeyValue_ReturnsSuccess,
    &Lex_SpacedKeyValue_LexedStrings,
    &Lex_SpacedKeyValue_LexedTokens,

    &Lex_NoSpaceKeyValue_ReturnsSuccess,
    &Lex_NoSpaceKeyValue_LexedStrings,
    &Lex_NoSpaceKeyValue_LexedTokens,

    &Lex_OnlySpaces_ReturnsSuccess,
    &Lex_OnlySpaces_OneString,
    &Lex_OnlySpaces_NoTokens,

    &Lex_Empty_ReturnsSuccess,
    &Lex_Empty_NoStrings,
    &Lex_MultipleEmpty_NoStrings,

    &Lex_CommentLine_ReturnsSuccess,
    &Lex_CommentLine_OneString,
    &Lex_CommentLine_NoTokens,

    &Lex_SetLineNumber_ReturnsSuccessWithLineNumber
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
