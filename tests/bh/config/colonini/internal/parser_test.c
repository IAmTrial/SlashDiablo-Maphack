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
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "bh/config/colonini/internal/lexer.h"
#include "bh/config/colonini/internal/lexer/lexer_string.h"
#include "bh/config/colonini/internal/parser.h"
#include "bh/config/colonini/internal/test_helper.h"

/* key: value */
static struct LexerString kStringValue_Strs[4];
static struct LexerLine kStringValue;
static struct Lexer kStringValueLexer;
/* key: 42 */
static struct LexerString kSIntValue_Strs[4];
static struct LexerLine kSIntValue;
static struct Lexer kSIntValueLexer;
/* key: 0xBeAd0928 */
static struct LexerString kUIntValue_Strs[4];
static struct LexerLine kUIntValue;
static struct Lexer kUIntValueLexer;
/* key: tRue */
static struct LexerString kBooleanTrueValue_Strs[4];
static struct LexerLine kBooleanTrueValue;
static struct Lexer kBooleanTrueValueLexer;
/* key: fAlsE */
static struct LexerString kBooleanFalseValue_Strs[4];
static struct LexerLine kBooleanFalseValue;
static struct Lexer kBooleanFalseValueLexer;
/* key: false, VK_RSHIFT */
static struct LexerString kToggleValue_Strs[7];
static struct LexerLine kToggleValue;
static struct Lexer kToggleValueLexer;
/* key: false to me, VK_RSHIFT */
static struct LexerString kLeftNotToggleValue_Strs[11];
static struct LexerLine kLeftNotToggleValue;
static struct Lexer kLeftNotToggleValueLexer;
/* key: false, VK_RSHIFT to me */
static struct LexerString kRightNotToggleValue_Strs[11];
static struct LexerLine kRightNotToggleValue;
static struct Lexer kRightNotToggleValueLexer;
/* key: */
static struct LexerString kEmptyValue_Strs[2];
static struct LexerLine kEmptyValue;
static struct Lexer kEmptyValueLexer;
/* \vkey\t:    value with\tstuff      */
static struct LexerString kSpacedKeyValue_Strs[11];
static struct LexerLine kSpacedKeyValue;
static struct Lexer kSpacedKeyValueLexer;
/* key[subkey]: value */
static struct LexerString kMappedKeyValue_Strs[7];
static struct LexerLine kMappedKeyValue;
static struct Lexer kMappedKeyValueLexer;
/* key key\t[  sub key]\v:val value */
static struct LexerString kSpacedMappedKeyValue_Strs[15];
static struct LexerLine kSpacedMappedKeyValue;
static struct Lexer kSpacedMappedKeyValueLexer;
/* key[subkey]:value */
static struct LexerString kNoSpaceMappedKeyValue_Strs[6];
static struct LexerLine kNoSpaceMappedKeyValue;
static struct Lexer kNoSpaceMappedKeyValueLexer;
/* key\t[  subkey1]\v[   sub key2  ][0]:value // comment */
static struct LexerString kMixedMappedKeyValue_Strs[21];
static struct LexerLine kMixedMappedKeyValue;
static struct Lexer kMixedMappedKeyValueLexer;
/* // Comment */
static struct LexerString kComment_Strs[1];
static struct LexerLine kComment;
static struct Lexer kCommentLexer;
/* \t     */
static struct LexerString kSpaces_Strs[1];
static struct LexerLine kSpaces;
static struct Lexer kSpacesLexer;
/* key[invalid:value */
static struct LexerString kInvalid_Strs[5];
static struct LexerLine kInvalid;
static struct Lexer kInvalidLexer;

static void BeforeAllSetUp(void) {
  size_t line_number;

  line_number = 0;
  LexerLineSetUp(
      &kStringValue,
      kStringValue_Strs,
      sizeof(kStringValue_Strs) / sizeof(kStringValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "value", 1);
  kStringValue.line_number = ++line_number;
  kStringValueLexer.count = 1;
  kStringValueLexer.lines = &kStringValue;

  LexerLineSetUp(
      &kSIntValue,
      kSIntValue_Strs,
      sizeof(kSIntValue_Strs) / sizeof(kSIntValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "42", 1);
  kSIntValue.line_number = ++line_number;
  kSIntValueLexer.count = 1;
  kSIntValueLexer.lines = &kSIntValue;

  LexerLineSetUp(
      &kUIntValue,
      kUIntValue_Strs,
      sizeof(kUIntValue_Strs) / sizeof(kUIntValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "0xBeAd0928", 1);
  kUIntValue.line_number = ++line_number;
  kUIntValueLexer.count = 1;
  kUIntValueLexer.lines = &kUIntValue;

  LexerLineSetUp(
      &kBooleanTrueValue,
      kBooleanTrueValue_Strs,
      sizeof(kBooleanTrueValue_Strs) / sizeof(kBooleanTrueValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "tRue", 1);
  kBooleanTrueValue.line_number = ++line_number;
  kBooleanTrueValueLexer.count = 1;
  kBooleanTrueValueLexer.lines = &kBooleanTrueValue;

  LexerLineSetUp(
      &kBooleanFalseValue,
      kBooleanFalseValue_Strs,
      sizeof(kBooleanFalseValue_Strs) / sizeof(kBooleanFalseValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "fAlsE", 1);
  kBooleanFalseValue.line_number = ++line_number;
  kBooleanFalseValueLexer.count = 1;
  kBooleanFalseValueLexer.lines = &kBooleanFalseValue;

  LexerLineSetUp(
      &kToggleValue,
      kToggleValue_Strs,
      sizeof(kToggleValue_Strs) / sizeof(kToggleValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "false", 1,
      ",", 1,
      " ", 0,
      "VK_RSHIFT", 1);
  kToggleValue.line_number = ++line_number;
  kToggleValueLexer.count = 1;
  kToggleValueLexer.lines = &kToggleValue;

  LexerLineSetUp(
      &kLeftNotToggleValue,
      kLeftNotToggleValue_Strs,
      sizeof(kLeftNotToggleValue_Strs) / sizeof(kLeftNotToggleValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "false", 1,
      " ", 0,
      "to", 1,
      " ", 0,
      "me", 1,
      ",", 1,
      " ", 0,
      "VK_RSHIFT", 1);
  kLeftNotToggleValue.line_number = ++line_number;
  kLeftNotToggleValueLexer.count = 1;
  kLeftNotToggleValueLexer.lines = &kLeftNotToggleValue;

  LexerLineSetUp(
      &kRightNotToggleValue,
      kRightNotToggleValue_Strs,
      sizeof(kRightNotToggleValue_Strs) / sizeof(kRightNotToggleValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "false", 1,
      ",", 1,
      " ", 0,
      "VK_RSHIFT", 1,
      " ", 0,
      "to", 1,
      " ", 0,
      "me", 1);
  kRightNotToggleValue.line_number = ++line_number;
  kRightNotToggleValueLexer.count = 1;
  kRightNotToggleValueLexer.lines = &kRightNotToggleValue;

  LexerLineSetUp(
      &kEmptyValue,
      kEmptyValue_Strs,
      sizeof(kEmptyValue_Strs) / sizeof(kEmptyValue_Strs[0]),
      "key", 1,
      ":", 1);
  kEmptyValue.line_number = ++line_number;
  kEmptyValueLexer.count = 1;
  kEmptyValueLexer.lines = &kEmptyValue;

  LexerLineSetUp(
      &kSpacedKeyValue,
      kSpacedKeyValue_Strs,
      sizeof(kSpacedKeyValue_Strs) / sizeof(kSpacedKeyValue_Strs[0]),
      "\v", 0,
      "key", 1,
      "\t", 0,
      ":", 1,
      "    ", 0,
      "value", 1,
      " ", 0,
      "with", 1,
      "\t", 0,
      "stuff", 1,
      "     ", 0);
  kSpacedKeyValue.line_number = ++line_number;
  kSpacedKeyValueLexer.count = 1;
  kSpacedKeyValueLexer.lines = &kSpacedKeyValue;

  LexerLineSetUp(
      &kMappedKeyValue,
      kMappedKeyValue_Strs,
      sizeof(kMappedKeyValue_Strs) / sizeof(kMappedKeyValue_Strs[0]),
      "key", 1,
      "[", 1,
      "subkey", 1,
      "]", 1,
      ":", 1,
      " ", 0,
      "value", 1);
  kMappedKeyValue.line_number = ++line_number;
  kMappedKeyValueLexer.count = 1;
  kMappedKeyValueLexer.lines = &kMappedKeyValue;

  LexerLineSetUp(
      &kSpacedMappedKeyValue,
      kSpacedMappedKeyValue_Strs,
      sizeof(kSpacedMappedKeyValue_Strs)
          / sizeof(kSpacedMappedKeyValue_Strs[0]),
      "key", 1,
      " ", 0,
      "key", 1,
      "\t", 0,
      "[", 1,
      "  ", 0,
      "sub", 1,
      " ", 0,
      "key", 1,
      "]", 1,
      "\v", 0,
      ":", 1,
      "val", 1,
      " ", 0,
      "value", 1);
  kSpacedMappedKeyValue.line_number = ++line_number;
  kSpacedMappedKeyValueLexer.count = 1;
  kSpacedMappedKeyValueLexer.lines = &kSpacedMappedKeyValue;

  LexerLineSetUp(
      &kNoSpaceMappedKeyValue,
      kNoSpaceMappedKeyValue_Strs,
      sizeof(kNoSpaceMappedKeyValue_Strs)
          / sizeof(kNoSpaceMappedKeyValue_Strs[0]),
      "key", 1,
      "[", 1,
      "subkey", 1,
      "]", 1,
      ":", 1,
      "value", 1);
  kNoSpaceMappedKeyValue.line_number = ++line_number;
  kNoSpaceMappedKeyValueLexer.count = 1;
  kNoSpaceMappedKeyValueLexer.lines = &kNoSpaceMappedKeyValue;

  LexerLineSetUp(
      &kMixedMappedKeyValue,
      kMixedMappedKeyValue_Strs,
      sizeof(kMixedMappedKeyValue_Strs) / sizeof(kMixedMappedKeyValue_Strs[0]),
      "key", 1,
      "\t", 0,
      "[", 1,
      "  ", 0,
      "subkey1", 1,
      "]", 1,
      "\v", 0,
      "[", 1,
      "   ", 0,
      "sub", 1,
      " ", 0,
      "key2", 1,
      "  ", 0,
      "]", 1,
      "[", 1,
      "0", 1,
      "]", 1,
      ":", 1,
      "value", 1,
      " ", 0,
      "// comment", 0);
  kMixedMappedKeyValue.line_number = ++line_number;
  kMixedMappedKeyValueLexer.count = 1;
  kMixedMappedKeyValueLexer.lines = &kMixedMappedKeyValue;

  LexerLineSetUp(
      &kComment,
      kComment_Strs,
      sizeof(kComment_Strs) / sizeof(kComment_Strs[0]),
      "// Comment", 0);
  kComment.line_number = ++line_number;
  kCommentLexer.count = 1;
  kCommentLexer.lines = &kComment;

  LexerLineSetUp(
      &kSpaces,
      kSpaces_Strs,
      sizeof(kSpaces_Strs) / sizeof(kSpaces_Strs[0]),
      "\t    ", 0);
  kSpaces.line_number = ++line_number;
  kSpacesLexer.count = 1;
  kSpacesLexer.lines = &kSpaces;

  LexerLineSetUp(
      &kInvalid,
      kInvalid_Strs,
      sizeof(kInvalid_Strs) / sizeof(kInvalid_Strs[0]),
      "key", 1,
      "[", 1,
      "invalid", 1,
      ":", 1,
      "value", 1);
  kInvalid.line_number = ++line_number;
  kInvalidLexer.count = 1;
  kInvalidLexer.lines = &kInvalid;
}

struct EachContext {
  struct Parser parser;
};

typedef void TestFunc(struct EachContext* context);

static void BeforeEach(struct EachContext* context) {
  struct Parser* init_result;

  init_result = Parser_Init(&context->parser, 16);
  assert(init_result == &context->parser);
}

static void AfterEach(struct EachContext* context) {
  Parser_Deinit(&context->parser);
}

static void Parse_StringValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(&context->parser, &kStringValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_StringValue_ParsedStrings(struct EachContext* context) {

  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kStringValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kString);
  assert(value_constexpr->length == 5);
  assert(strcmp(value_constexpr->expr, "value") == 0);
}

static void Parse_StringValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kStringValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kStringValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kStringValue.strs[1]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kStringValue.strs[3]);
  assert(value_constexpr->end_src == &kStringValue.strs[4]);
}

static void Parse_SIntValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(&context->parser, &kSIntValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_SIntValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kSIntValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kSignedInt);
  assert(value_constexpr->length == 2);
  assert(strcmp(value_constexpr->expr, "42") == 0);
}

static void Parse_SIntValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kSIntValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kSIntValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kSIntValue.strs[1]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kSIntValue.strs[3]);
  assert(value_constexpr->end_src == &kSIntValue.strs[4]);
}

static void Parse_UIntValue_Success(struct EachContext* context) {
  int parse_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_result =
      Parser_Parse(&context->parser, &kUIntValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_UIntValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kUIntValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kUnsignedInt);
  assert(value_constexpr->length == 10);
  assert(strcmp(value_constexpr->expr, "0xBeAd0928") == 0);
}

static void Parse_UIntValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kUIntValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kUIntValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kUIntValue.strs[1]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kUIntValue.strs[3]);
  assert(value_constexpr->end_src == &kUIntValue.strs[4]);
}

static void Parse_BooleanTrueValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(&context->parser, &kBooleanTrueValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_BooleanTrueValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kBooleanTrueValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kBoolean);
  assert(value_constexpr->length == 4);
  assert(strcmp(value_constexpr->expr, "tRue") == 0);
}

static void Parse_BooleanTrueValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kBooleanTrueValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kBooleanTrueValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kBooleanTrueValue.strs[1]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kBooleanTrueValue.strs[3]);
  assert(value_constexpr->end_src == &kBooleanTrueValue.strs[4]);
}

static void Parse_BooleanFalseValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(&context->parser, &kBooleanFalseValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_BooleanFalseValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kBooleanFalseValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kBoolean);
  assert(value_constexpr->length == 5);
  assert(strcmp(value_constexpr->expr, "fAlsE") == 0);
}

static void Parse_BooleanFalseValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kBooleanFalseValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kBooleanFalseValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kBooleanFalseValue.strs[1]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kBooleanFalseValue.strs[3]);
  assert(value_constexpr->end_src == &kBooleanFalseValue.strs[4]);
}

static void Parse_ToggleValue_Success(struct EachContext* context) {
  int parse_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_result =
      Parser_Parse(&context->parser, &kToggleValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_ToggleValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ToggleExpr* value_toggleexpr;
  struct ConstExpr* value_enabled;
  struct ConstExpr* value_input;

  Parser_Parse(&context->parser, &kToggleValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kToggle);
  value_toggleexpr = &value_expr->variant.as_toggleexpr;
  value_enabled = &value_toggleexpr->enabled_expr;
  assert(value_enabled->type == ConstExprType_kBoolean);
  assert(value_enabled->length == 5);
  assert(strcmp(value_enabled->expr, "false") == 0);
  value_input = &value_toggleexpr->input_expr;
  assert(value_input->type == ConstExprType_kString);
  assert(value_input->length == 9);
  assert(strcmp(value_input->expr, "VK_RSHIFT") == 0);
}

static void Parse_ToggleValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ToggleExpr* value_toggleexpr;
  struct ConstExpr* value_enabled;
  struct ConstExpr* value_input;

  Parser_Parse(&context->parser, &kToggleValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kToggleValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kToggleValue.strs[1]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_toggleexpr = &value_expr->variant.as_toggleexpr;
  value_enabled = &value_toggleexpr->enabled_expr;
  assert(value_enabled->begin_src == &kToggleValue.strs[3]);
  assert(value_enabled->end_src == &kToggleValue.strs[4]);
  value_input = &value_toggleexpr->input_expr;
  assert(value_input->begin_src == &kToggleValue.strs[6]);
  assert(value_input->end_src == &kToggleValue.strs[7]);
}

static void Parse_LeftNotToggleValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(
          &context->parser, &kLeftNotToggleValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_LeftNotToggleValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kLeftNotToggleValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kString);
  assert(value_constexpr->length == 22);
  assert(strcmp(value_constexpr->expr, "false to me, VK_RSHIFT") == 0);
}

static void Parse_LeftNotToggleValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kLeftNotToggleValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kLeftNotToggleValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kLeftNotToggleValue.strs[1]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kLeftNotToggleValue.strs[3]);
  assert(value_constexpr->end_src == &kLeftNotToggleValue.strs[11]);
}

static void Parse_RightNotToggleValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(
          &context->parser, &kRightNotToggleValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_RightNotToggleValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kRightNotToggleValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kString);
  assert(value_constexpr->length == 22);
  assert(strcmp(value_constexpr->expr, "false, VK_RSHIFT to me") == 0);
}

static void Parse_RightNotToggleValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kRightNotToggleValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kRightNotToggleValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kRightNotToggleValue.strs[1]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kRightNotToggleValue.strs[3]);
  assert(value_constexpr->end_src == &kRightNotToggleValue.strs[11]);
}

static void Parse_EmptyValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(&context->parser, &kEmptyValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_EmptyValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kEmptyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kEmpty);
}

static void Parse_EmptyValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kEmptyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kEmptyValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kEmptyValue.strs[1]);
}

static void Parse_SpacedKeyValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(&context->parser, &kSpacedKeyValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_SpacedKeyValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kSpacedKeyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->length == 16);
  assert(strcmp(value_constexpr->expr, "value with\tstuff") == 0);
}

static void Parse_SpacedKeyValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kSpacedKeyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kSpacedKeyValue.strs[1]);
  assert(key_expr->constexpr.end_src == &kSpacedKeyValue.strs[2]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kSpacedKeyValue.strs[5]);
  assert(value_constexpr->end_src == &kSpacedKeyValue.strs[10]);
}

static void Parse_MappedKeyValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(&context->parser, &kMappedKeyValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_MappedKeyValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kMappedKeyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 1);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.length == 6);
  assert(strcmp(subscripts[0].expr.expr, "subkey") == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->length == 5);
  assert(strcmp(value_constexpr->expr, "value") == 0);
}

static void Parse_MappedKeyValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kMappedKeyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kMappedKeyValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kMappedKeyValue.strs[1]);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.begin_src == &kMappedKeyValue.strs[2]);
  assert(subscripts[0].expr.end_src == &kMappedKeyValue.strs[3]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kMappedKeyValue.strs[6]);
  assert(value_constexpr->end_src == &kMappedKeyValue.strs[7]);
}

static void Parse_SpacedMappedKeyValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(
          &context->parser, &kSpacedMappedKeyValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_SpacedMappedKeyValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kSpacedMappedKeyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 7);
  assert(strcmp(key_expr->constexpr.expr, "key key") == 0);
  assert(key_expr->subscripts_count == 1);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.length == 7);
  assert(strcmp(subscripts[0].expr.expr, "sub key") == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->length == 9);
  assert(strcmp(value_constexpr->expr, "val value") == 0);
}

static void Parse_SpacedMappedKeyValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kSpacedMappedKeyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kSpacedMappedKeyValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kSpacedMappedKeyValue.strs[3]);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.begin_src == &kSpacedMappedKeyValue.strs[6]);
  assert(subscripts[0].expr.end_src == &kSpacedMappedKeyValue.strs[9]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kSpacedMappedKeyValue.strs[12]);
  assert(value_constexpr->end_src == &kSpacedMappedKeyValue.strs[15]);
}

static void Parse_NoSpaceMappedKeyValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(
          &context->parser, &kNoSpaceMappedKeyValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_NoSpaceMappedKeyValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kNoSpaceMappedKeyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 1);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.length == 6);
  assert(strcmp(subscripts[0].expr.expr, "subkey") == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->length == 5);
  assert(strcmp(value_constexpr->expr, "value") == 0);
}

static void Parse_NoSpaceMappedKeyValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kNoSpaceMappedKeyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kNoSpaceMappedKeyValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kNoSpaceMappedKeyValue.strs[1]);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.begin_src == &kNoSpaceMappedKeyValue.strs[2]);
  assert(subscripts[0].expr.end_src == &kNoSpaceMappedKeyValue.strs[3]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kNoSpaceMappedKeyValue.strs[5]);
  assert(value_constexpr->end_src == &kNoSpaceMappedKeyValue.strs[6]);
}

static void Parse_MixedMappedKeyValue_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(
          &context->parser, &kMixedMappedKeyValueLexer, &error_column);

  assert(parse_result);
}

static void Parse_MixedMappedKeyValue_ParsedStrings(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kMixedMappedKeyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 3);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.type == ConstExprType_kString);
  assert(subscripts[0].expr.length == 7);
  assert(strcmp(subscripts[0].expr.expr, "subkey1") == 0);
  assert(subscripts[1].expr.type == ConstExprType_kString);
  assert(subscripts[1].expr.length == 8);
  assert(strcmp(subscripts[1].expr.expr, "sub key2") == 0);
  assert(subscripts[2].expr.type == ConstExprType_kString);
  assert(subscripts[2].expr.length == 1);
  assert(strcmp(subscripts[2].expr.expr, "0") == 0);
  value_expr = &pline->variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->length == 5);
  assert(strcmp(value_constexpr->expr, "value") == 0);
}

static void Parse_MixedMappedKeyValue_SetSources(struct EachContext* context) {
  size_t error_column;

  struct ParserLine* pline;
  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  Parser_Parse(&context->parser, &kMixedMappedKeyValueLexer, &error_column);

  pline = &context->parser.lines[0];
  key_expr = &pline->variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kMixedMappedKeyValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kMixedMappedKeyValue.strs[1]);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.begin_src == &kMixedMappedKeyValue.strs[4]);
  assert(subscripts[0].expr.end_src == &kMixedMappedKeyValue.strs[5]);
  assert(subscripts[1].expr.begin_src == &kMixedMappedKeyValue.strs[9]);
  assert(subscripts[1].expr.end_src == &kMixedMappedKeyValue.strs[12]);
  assert(subscripts[2].expr.begin_src == &kMixedMappedKeyValue.strs[15]);
  assert(subscripts[2].expr.end_src == &kMixedMappedKeyValue.strs[16]);
  value_expr = &pline->variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kMixedMappedKeyValue.strs[18]);
  assert(value_constexpr->end_src == &kMixedMappedKeyValue.strs[19]);
}

static void Parse_Comment_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(&context->parser, &kCommentLexer, &error_column);

  assert(parse_result);
}

static void Parse_Comment_NoOp(struct EachContext* context) {
  size_t error_column;

  Parser_Parse(&context->parser, &kCommentLexer, &error_column);

  assert(context->parser.lines[0].type == ParserLineType_kNoOp);
}

static void Parse_Spaces_Success(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result = Parser_Parse(&context->parser, &kSpacesLexer, &error_column);

  assert(parse_result);
}

static void Parse_Spaces_NoOp(struct EachContext* context) {
  size_t error_column;

  Parser_Parse(&context->parser, &kSpacesLexer, &error_column);

  assert(context->parser.lines[0].type == ParserLineType_kNoOp);
}

static void Parse_Invalid_SetErrorColumn(struct EachContext* context) {
  int parse_result;

  size_t error_column;

  parse_result =
      Parser_Parse(&context->parser, &kInvalidLexer, &error_column);

  assert(!parse_result);
  assert(error_column == 3);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &Parse_StringValue_Success,
    &Parse_StringValue_ParsedStrings,
    &Parse_StringValue_SetSources,

    &Parse_SIntValue_Success,
    &Parse_SIntValue_ParsedStrings,
    &Parse_SIntValue_SetSources,

    &Parse_UIntValue_Success,
    &Parse_UIntValue_ParsedStrings,
    &Parse_UIntValue_SetSources,

    &Parse_BooleanTrueValue_Success,
    &Parse_BooleanTrueValue_ParsedStrings,
    &Parse_BooleanTrueValue_SetSources,

    &Parse_BooleanFalseValue_Success,
    &Parse_BooleanFalseValue_ParsedStrings,
    &Parse_BooleanFalseValue_SetSources,

    &Parse_ToggleValue_Success,
    &Parse_ToggleValue_ParsedStrings,
    &Parse_ToggleValue_SetSources,

    &Parse_LeftNotToggleValue_Success,
    &Parse_LeftNotToggleValue_ParsedStrings,
    &Parse_LeftNotToggleValue_SetSources,

    &Parse_RightNotToggleValue_Success,
    &Parse_RightNotToggleValue_ParsedStrings,
    &Parse_RightNotToggleValue_SetSources,

    &Parse_EmptyValue_Success,
    &Parse_EmptyValue_ParsedStrings,
    &Parse_EmptyValue_SetSources,

    &Parse_SpacedKeyValue_Success,
    &Parse_SpacedKeyValue_ParsedStrings,
    &Parse_SpacedKeyValue_SetSources,

    &Parse_MappedKeyValue_Success,
    &Parse_MappedKeyValue_ParsedStrings,
    &Parse_MappedKeyValue_SetSources,

    &Parse_SpacedMappedKeyValue_Success,
    &Parse_SpacedMappedKeyValue_ParsedStrings,
    &Parse_SpacedMappedKeyValue_SetSources,

    &Parse_NoSpaceMappedKeyValue_Success,
    &Parse_NoSpaceMappedKeyValue_ParsedStrings,
    &Parse_NoSpaceMappedKeyValue_SetSources,

    &Parse_MixedMappedKeyValue_Success,
    &Parse_MixedMappedKeyValue_ParsedStrings,
    &Parse_MixedMappedKeyValue_SetSources,

    &Parse_Comment_Success,
    &Parse_Comment_NoOp,

    &Parse_Spaces_Success,
    &Parse_Spaces_NoOp,

    &Parse_Invalid_SetErrorColumn
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
