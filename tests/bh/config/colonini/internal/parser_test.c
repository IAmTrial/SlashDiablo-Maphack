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
/* key: 42 */
static struct LexerString kSIntValue_Strs[4];
static struct LexerLine kSIntValue;
/* key: 0xBeAd0928 */
static struct LexerString kUIntValue_Strs[4];
static struct LexerLine kUIntValue;
/* key: tRue */
static struct LexerString kBooleanTrueValue_Strs[4];
static struct LexerLine kBooleanTrueValue;
/* key: fAlsE */
static struct LexerString kBooleanFalseValue_Strs[4];
static struct LexerLine kBooleanFalseValue;
/* key: false, VK_RSHIFT */
static struct LexerString kToggleValue_Strs[7];
static struct LexerLine kToggleValue;
/* key: false to me, VK_RSHIFT */
static struct LexerString kLeftNotToggleValue_Strs[11];
static struct LexerLine kLeftNotToggleValue;
/* key: false, VK_RSHIFT to me */
static struct LexerString kRightNotToggleValue_Strs[11];
static struct LexerLine kRightNotToggleValue;
/* key: */
static struct LexerString kEmptyValue_Strs[2];
static struct LexerLine kEmptyValue;
/* \vkey\t:    value with\tstuff      */
static struct LexerString kSpacedKeyValue_Strs[11];
static struct LexerLine kSpacedKeyValue;
/* key[subkey]: value */
static struct LexerString kMappedKeyValue_Strs[7];
static struct LexerLine kMappedKeyValue;
/* key key\t[  sub key]\v:val value */
static struct LexerString kSpacedMappedKeyValue_Strs[15];
static struct LexerLine kSpacedMappedKeyValue;
/* key[subkey]:value */
static struct LexerString kNoSpaceMappedKeyValue_Strs[6];
static struct LexerLine kNoSpaceMappedKeyValue;
/* key\t[  subkey1]\v[   sub key2  ][0]:value // comment */
static struct LexerString kMixedMappedKeyValue_Strs[21];
static struct LexerLine kMixedMappedKeyValue;
/* // Comment */
static struct LexerString kComment_Strs[1];
static struct LexerLine kComment;
/* \t     */
static struct LexerString kSpaces_Strs[1];
static struct LexerLine kSpaces;
/* key[invalid:value */
static struct LexerString kInvalid_Strs[5];
static struct LexerLine kInvalid;

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

  LexerLineSetUp(
      &kSIntValue,
      kSIntValue_Strs,
      sizeof(kSIntValue_Strs) / sizeof(kSIntValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "42", 1);
  kSIntValue.line_number = ++line_number;

  LexerLineSetUp(
      &kUIntValue,
      kUIntValue_Strs,
      sizeof(kUIntValue_Strs) / sizeof(kUIntValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "0xBeAd0928", 1);
  kUIntValue.line_number = ++line_number;

  LexerLineSetUp(
      &kBooleanTrueValue,
      kBooleanTrueValue_Strs,
      sizeof(kBooleanTrueValue_Strs) / sizeof(kBooleanTrueValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "tRue", 1);
  kBooleanTrueValue.line_number = ++line_number;

  LexerLineSetUp(
      &kBooleanFalseValue,
      kBooleanFalseValue_Strs,
      sizeof(kBooleanFalseValue_Strs) / sizeof(kBooleanFalseValue_Strs[0]),
      "key", 1,
      ":", 1,
      " ", 0,
      "fAlsE", 1);
  kBooleanFalseValue.line_number = ++line_number;

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

  LexerLineSetUp(
      &kEmptyValue,
      kEmptyValue_Strs,
      sizeof(kEmptyValue_Strs) / sizeof(kEmptyValue_Strs[0]),
      "key", 1,
      ":", 1);
  kEmptyValue.line_number = ++line_number;

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

  LexerLineSetUp(
      &kComment,
      kComment_Strs,
      sizeof(kComment_Strs) / sizeof(kComment_Strs[0]),
      "// Comment", 0);
  kComment.line_number = ++line_number;

  LexerLineSetUp(
      &kSpaces,
      kSpaces_Strs,
      sizeof(kSpaces_Strs) / sizeof(kSpaces_Strs[0]),
      "\t    ", 0);
  kSpaces.line_number = ++line_number;

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
}

typedef void TestFunc(void);

static void ParseLine_StringValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kStringValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_StringValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kStringValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kString);
  assert(value_constexpr->length == 5);
  assert(strcmp(value_constexpr->expr, "value") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_StringValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kStringValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kStringValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kStringValue.strs[1]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kStringValue.strs[3]);
  assert(value_constexpr->end_src == &kStringValue.strs[4]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_SIntValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kSIntValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_SIntValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kSIntValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kSignedInt);
  assert(value_constexpr->length == 2);
  assert(strcmp(value_constexpr->expr, "42") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_SIntValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kSIntValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kSIntValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kSIntValue.strs[1]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kSIntValue.strs[3]);
  assert(value_constexpr->end_src == &kSIntValue.strs[4]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_UIntValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kUIntValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_UIntValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kUIntValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kUnsignedInt);
  assert(value_constexpr->length == 10);
  assert(strcmp(value_constexpr->expr, "0xBeAd0928") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_UIntValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kUIntValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kUIntValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kUIntValue.strs[1]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kUIntValue.strs[3]);
  assert(value_constexpr->end_src == &kUIntValue.strs[4]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_BooleanTrueValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kBooleanTrueValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_BooleanTrueValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kBooleanTrueValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kBoolean);
  assert(value_constexpr->length == 4);
  assert(strcmp(value_constexpr->expr, "tRue") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_BooleanTrueValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kBooleanTrueValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kBooleanTrueValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kBooleanTrueValue.strs[1]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kBooleanTrueValue.strs[3]);
  assert(value_constexpr->end_src == &kBooleanTrueValue.strs[4]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_BooleanFalseValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kBooleanFalseValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_BooleanFalseValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kBooleanFalseValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kBoolean);
  assert(value_constexpr->length == 5);
  assert(strcmp(value_constexpr->expr, "fAlsE") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_BooleanFalseValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kBooleanFalseValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kBooleanFalseValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kBooleanFalseValue.strs[1]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kBooleanFalseValue.strs[3]);
  assert(value_constexpr->end_src == &kBooleanFalseValue.strs[4]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_ToggleValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kToggleValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_ToggleValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ToggleExpr* value_toggleexpr;
  struct ConstExpr* value_enabled;
  struct ConstExpr* value_input;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kToggleValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
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

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_ToggleValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ToggleExpr* value_toggleexpr;
  struct ConstExpr* value_enabled;
  struct ConstExpr* value_input;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kToggleValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kToggleValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kToggleValue.strs[1]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_toggleexpr = &value_expr->variant.as_toggleexpr;
  value_enabled = &value_toggleexpr->enabled_expr;
  assert(value_enabled->begin_src == &kToggleValue.strs[3]);
  assert(value_enabled->end_src == &kToggleValue.strs[4]);
  value_input = &value_toggleexpr->input_expr;
  assert(value_input->begin_src == &kToggleValue.strs[6]);
  assert(value_input->end_src == &kToggleValue.strs[7]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_LeftNotToggleValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kLeftNotToggleValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_LeftNotToggleValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kLeftNotToggleValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kString);
  assert(value_constexpr->length == 22);
  assert(strcmp(value_constexpr->expr, "false to me, VK_RSHIFT") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_LeftNotToggleValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kLeftNotToggleValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kLeftNotToggleValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kLeftNotToggleValue.strs[1]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kLeftNotToggleValue.strs[3]);
  assert(value_constexpr->end_src == &kLeftNotToggleValue.strs[11]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_RightNotToggleValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kRightNotToggleValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_RightNotToggleValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kRightNotToggleValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->type == ConstExprType_kString);
  assert(value_constexpr->length == 22);
  assert(strcmp(value_constexpr->expr, "false, VK_RSHIFT to me") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_RightNotToggleValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kRightNotToggleValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kRightNotToggleValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kRightNotToggleValue.strs[1]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kRightNotToggleValue.strs[3]);
  assert(value_constexpr->end_src == &kRightNotToggleValue.strs[11]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_EmptyValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kEmptyValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_EmptyValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kEmptyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kEmpty);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_EmptyValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kEmptyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kEmptyValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kEmptyValue.strs[1]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_SpacedKeyValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kSpacedKeyValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_SpacedKeyValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kSpacedKeyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->length == 16);
  assert(strcmp(value_constexpr->expr, "value with\tstuff") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_SpacedKeyValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kSpacedKeyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kSpacedKeyValue.strs[1]);
  assert(key_expr->constexpr.end_src == &kSpacedKeyValue.strs[2]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kSpacedKeyValue.strs[5]);
  assert(value_constexpr->end_src == &kSpacedKeyValue.strs[10]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_MappedKeyValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kMappedKeyValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_MappedKeyValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kMappedKeyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 1);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.length == 6);
  assert(strcmp(subscripts[0].expr.expr, "subkey") == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->length == 5);
  assert(strcmp(value_constexpr->expr, "value") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_MappedKeyValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kMappedKeyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kMappedKeyValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kMappedKeyValue.strs[1]);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.begin_src == &kMappedKeyValue.strs[2]);
  assert(subscripts[0].expr.end_src == &kMappedKeyValue.strs[3]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kMappedKeyValue.strs[6]);
  assert(value_constexpr->end_src == &kMappedKeyValue.strs[7]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_SpacedMappedKeyValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(
          &parser_line, &kSpacedMappedKeyValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_SpacedMappedKeyValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(
          &parser_line, &kSpacedMappedKeyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 7);
  assert(strcmp(key_expr->constexpr.expr, "key key") == 0);
  assert(key_expr->subscripts_count == 1);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.length == 7);
  assert(strcmp(subscripts[0].expr.expr, "sub key") == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->length == 9);
  assert(strcmp(value_constexpr->expr, "val value") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_SpacedMappedKeyValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(
          &parser_line, &kSpacedMappedKeyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kSpacedMappedKeyValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kSpacedMappedKeyValue.strs[3]);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.begin_src == &kSpacedMappedKeyValue.strs[6]);
  assert(subscripts[0].expr.end_src == &kSpacedMappedKeyValue.strs[9]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kSpacedMappedKeyValue.strs[12]);
  assert(value_constexpr->end_src == &kSpacedMappedKeyValue.strs[15]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_NoSpaceMappedKeyValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(
          &parser_line, &kNoSpaceMappedKeyValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_NoSpaceMappedKeyValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(
          &parser_line, &kNoSpaceMappedKeyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.type == ConstExprType_kString);
  assert(key_expr->constexpr.length == 3);
  assert(strcmp(key_expr->constexpr.expr, "key") == 0);
  assert(key_expr->subscripts_count == 1);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.length == 6);
  assert(strcmp(subscripts[0].expr.expr, "subkey") == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->length == 5);
  assert(strcmp(value_constexpr->expr, "value") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_NoSpaceMappedKeyValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(
          &parser_line, &kNoSpaceMappedKeyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kNoSpaceMappedKeyValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kNoSpaceMappedKeyValue.strs[1]);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.begin_src == &kNoSpaceMappedKeyValue.strs[2]);
  assert(subscripts[0].expr.end_src == &kNoSpaceMappedKeyValue.strs[3]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kNoSpaceMappedKeyValue.strs[5]);
  assert(value_constexpr->end_src == &kNoSpaceMappedKeyValue.strs[6]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_MixedMappedKeyValue_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(
          &parser_line, &kMixedMappedKeyValue, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_MixedMappedKeyValue_ParsedStrings(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(
          &parser_line, &kMixedMappedKeyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
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
  assert(subscripts[2].expr.type == ConstExprType_kSignedInt);
  assert(subscripts[2].expr.length == 1);
  assert(strcmp(subscripts[2].expr.expr, "0") == 0);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  assert(value_expr->type == ValueExprType_kConst);
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->length == 5);
  assert(strcmp(value_constexpr->expr, "value") == 0);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_MixedMappedKeyValue_SetSources(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  struct KeyExpr* key_expr;
  struct Subscript* subscripts;
  struct ValueExpr* value_expr;
  struct ConstExpr* value_constexpr;

  parse_line_result =
      ParserLine_ParseLine(
          &parser_line, &kMixedMappedKeyValue, &error_column);

  key_expr = &parser_line.variant.assign_statement.key_expr;
  assert(key_expr->constexpr.begin_src == &kMixedMappedKeyValue.strs[0]);
  assert(key_expr->constexpr.end_src == &kMixedMappedKeyValue.strs[1]);
  subscripts = key_expr->subscripts;
  assert(subscripts[0].expr.begin_src == &kMixedMappedKeyValue.strs[4]);
  assert(subscripts[0].expr.end_src == &kMixedMappedKeyValue.strs[5]);
  assert(subscripts[1].expr.begin_src == &kMixedMappedKeyValue.strs[9]);
  assert(subscripts[1].expr.end_src == &kMixedMappedKeyValue.strs[12]);
  assert(subscripts[2].expr.begin_src == &kMixedMappedKeyValue.strs[15]);
  assert(subscripts[2].expr.end_src == &kMixedMappedKeyValue.strs[16]);
  value_expr = &parser_line.variant.assign_statement.value_expr;
  value_constexpr = &value_expr->variant.as_constexpr;
  assert(value_constexpr->begin_src == &kMixedMappedKeyValue.strs[18]);
  assert(value_constexpr->end_src == &kMixedMappedKeyValue.strs[19]);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_Comment_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kComment, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_Comment_NoOp(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kComment, &error_column);

  assert(parser_line.type == ParserLineType_kNoOp);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_Spaces_Success(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kSpaces, &error_column);

  assert(parse_line_result == &parser_line);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_Spaces_NoOp(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kSpaces, &error_column);

  assert(parser_line.type == ParserLineType_kNoOp);

  ParserLine_Deinit(&parser_line);
}

static void ParseLine_Invalid_SetErrorColumn(void) {
  struct ParserLine* parse_line_result;

  struct ParserLine parser_line;
  size_t error_column;

  parse_line_result =
      ParserLine_ParseLine(&parser_line, &kInvalid, &error_column);

  assert(parse_line_result == NULL);
  assert(error_column == 3);

  ParserLine_Deinit(&parser_line);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &ParseLine_StringValue_Success,
    &ParseLine_StringValue_ParsedStrings,
    &ParseLine_StringValue_SetSources,

    &ParseLine_SIntValue_Success,
    &ParseLine_SIntValue_ParsedStrings,
    &ParseLine_SIntValue_SetSources,

    &ParseLine_UIntValue_Success,
    &ParseLine_UIntValue_ParsedStrings,
    &ParseLine_UIntValue_SetSources,

    &ParseLine_BooleanTrueValue_Success,
    &ParseLine_BooleanTrueValue_ParsedStrings,
    &ParseLine_BooleanTrueValue_SetSources,

    &ParseLine_BooleanFalseValue_Success,
    &ParseLine_BooleanFalseValue_ParsedStrings,
    &ParseLine_BooleanFalseValue_SetSources,

    &ParseLine_ToggleValue_Success,
    &ParseLine_ToggleValue_ParsedStrings,
    &ParseLine_ToggleValue_SetSources,

    &ParseLine_LeftNotToggleValue_Success,
    &ParseLine_LeftNotToggleValue_ParsedStrings,
    &ParseLine_LeftNotToggleValue_SetSources,

    &ParseLine_RightNotToggleValue_Success,
    &ParseLine_RightNotToggleValue_ParsedStrings,
    &ParseLine_RightNotToggleValue_SetSources,

    &ParseLine_EmptyValue_Success,
    &ParseLine_EmptyValue_ParsedStrings,
    &ParseLine_EmptyValue_SetSources,

    &ParseLine_SpacedKeyValue_Success,
    &ParseLine_SpacedKeyValue_ParsedStrings,
    &ParseLine_SpacedKeyValue_SetSources,

    &ParseLine_MappedKeyValue_Success,
    &ParseLine_MappedKeyValue_ParsedStrings,
    &ParseLine_MappedKeyValue_SetSources,

    &ParseLine_SpacedMappedKeyValue_Success,
    &ParseLine_SpacedMappedKeyValue_ParsedStrings,
    &ParseLine_SpacedMappedKeyValue_SetSources,

    &ParseLine_NoSpaceMappedKeyValue_Success,
    &ParseLine_NoSpaceMappedKeyValue_ParsedStrings,
    &ParseLine_NoSpaceMappedKeyValue_SetSources,

    &ParseLine_MixedMappedKeyValue_Success,
    &ParseLine_MixedMappedKeyValue_ParsedStrings,
    &ParseLine_MixedMappedKeyValue_SetSources,

    &ParseLine_Comment_Success,
    &ParseLine_Comment_NoOp,

    &ParseLine_Spaces_Success,
    &ParseLine_Spaces_NoOp,

    &ParseLine_Invalid_SetErrorColumn
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
