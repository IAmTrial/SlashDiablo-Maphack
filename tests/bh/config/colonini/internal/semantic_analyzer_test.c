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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bh/config/colonini/internal/parser.h"
#include "bh/config/colonini/internal/test_helper.h"
#include "bh/config/colonini/internal/semantic_analyzer.h"

typedef void TestFunc(void);

static void BeforeAllSetUp(void) {
}

static void LoadLines_TwoKeys_Success(void) {
  int actual;
  struct SemanticAnalyzer analyzer;
  struct ParserLine lines[9];
  struct Subscript subscripts[9][1];
  struct Parser parser;

  /*
   * key01: value
   * key02: 42
   * key03: 0x1
   * key04: true
   * key05: true, VK_A
   * key06[subkey]: value
   * key07[42]: value
   * key08[0x1]: value
   * key09[true]: value
   */
  parser.lines = lines;
  parser.line_count = sizeof(lines) / sizeof(lines[0]);
  ParserLineBasicSetUp(&lines[0], "value", "key01", subscripts[0], 0);
  ParserLineBasicSetUp(&lines[1], "42", "key02", subscripts[1], 0);
  lines[1].variant.assign_statement.value_expr.variant.as_constexpr.type =
      ConstExprType_kSignedInt;
  ParserLineBasicSetUp(&lines[2], "0x1", "key03", subscripts[2], 0);
  lines[2].variant.assign_statement.value_expr.variant.as_constexpr.type =
      ConstExprType_kUnsignedInt;
  ParserLineBasicSetUp(&lines[3], "true", "key04", subscripts[3], 0);
  lines[3].variant.assign_statement.value_expr.variant.as_constexpr.type =
      ConstExprType_kBoolean;
  ParserLineBasicSetUp(&lines[4], "true, VK_A", "key05", subscripts[4], 0);
  lines[4].variant.assign_statement.value_expr.type = ValueExprType_kToggle;
  ParserLineBasicSetUp(&lines[5], "value", "key06", subscripts[5], 1, "subkey");
  ParserLineBasicSetUp(&lines[6], "value", "key07", subscripts[6], 1, "42");
  lines[6].variant.assign_statement.key_expr.subscripts[0].expr.type =
      ConstExprType_kSignedInt;
  ParserLineBasicSetUp(&lines[7], "value", "key08", subscripts[7], 1, "0x1");
  lines[7].variant.assign_statement.key_expr.subscripts[0].expr.type =
      ConstExprType_kUnsignedInt;
  ParserLineBasicSetUp(&lines[8], "value", "key09", subscripts[8], 1, "true");
  lines[8].variant.assign_statement.key_expr.subscripts[0].expr.type =
      ConstExprType_kBoolean;
  SemanticAnalyzer_Init(&analyzer, &parser);

  actual = SemanticAnalyzer_LoadLines(&analyzer, &parser);

  assert(actual);
  assert(analyzer.typing_table.typings[0].value_type == ValueExprType_kConst);
  assert(
      analyzer.typing_table.typings[0].value_as_constexpr_type
          == ConstExprType_kString);
  assert(
      analyzer.typing_table.typings[1].value_type == ValueExprType_kConst);
  assert(
      analyzer.typing_table.typings[1].value_as_constexpr_type
          == ConstExprType_kSignedInt);
  assert(analyzer.typing_table.typings[2].value_type == ValueExprType_kConst);
  assert(
      analyzer.typing_table.typings[2].value_as_constexpr_type
          == ConstExprType_kUnsignedInt);
  assert(analyzer.typing_table.typings[3].value_type == ValueExprType_kConst);
  assert(
      analyzer.typing_table.typings[3].value_as_constexpr_type
          == ConstExprType_kBoolean);
  assert(
      analyzer.typing_table.typings[4].value_type
          == ValueExprType_kToggle);
  assert(analyzer.typing_table.typings[5].subkey_type_count == 1);
  assert(
      analyzer.typing_table.typings[5].subkey_types[0]
          == ConstExprType_kString);
  assert(
      analyzer.typing_table.typings[6].subkey_types[0]
          == ConstExprType_kSignedInt);
  assert(
      analyzer.typing_table.typings[7].subkey_types[0]
          == ConstExprType_kUnsignedInt);
  assert(
      analyzer.typing_table.typings[8].subkey_types[0]
          == ConstExprType_kBoolean);

  SemanticAnalyzer_Deinit(&analyzer);
}

static void LoadLines_DuplicateNames_Failure(void) {
  int actual;
  struct SemanticAnalyzer analyzer;
  struct ParserLine lines[2];
  struct Subscript subscripts[2][1];
  struct Parser parser;

  /*
   * key: value1
   * key: value2
   */
  parser.lines = lines;
  parser.line_count = sizeof(lines) / sizeof(lines[0]);
  ParserLineBasicSetUp(&lines[0], "value1", "key", subscripts[0], 0);
  ParserLineBasicSetUp(&lines[1], "value2", "key", subscripts[1], 0);
  SemanticAnalyzer_Init(&analyzer, &parser);

  actual = SemanticAnalyzer_LoadLines(&analyzer, &parser);

  assert(!actual);

  SemanticAnalyzer_Deinit(&analyzer);
}

static void LoadLines_DuplicateSubkeys_Failure(void) {
  int actual;
  struct SemanticAnalyzer analyzer;
  struct ParserLine lines[2];
  struct Subscript subscripts[2][1];
  struct Parser parser;

  /*
   * key[subkey]: value1
   * key[subkey]: value2
   */
  parser.lines = lines;
  parser.line_count = sizeof(lines) / sizeof(lines[0]);
  ParserLineBasicSetUp(
      &lines[0], "value1", "key", subscripts[0], 1, "subkey");
  ParserLineBasicSetUp(
      &lines[1], "value2", "key", subscripts[1], 1, "subkey");
  SemanticAnalyzer_Init(&analyzer, &parser);

  actual = SemanticAnalyzer_LoadLines(&analyzer, &parser);

  assert(!actual);

  SemanticAnalyzer_Deinit(&analyzer);
}

static void LoadLines_SameKeyMismatch_Failure(void) {
  int actual;
  struct SemanticAnalyzer analyzer;
  struct ParserLine lines[2];
  struct Subscript subscripts[2][1];
  struct Parser parser;

  /*
   * key[subkey]: value
   * key: value
   */
  parser.lines = lines;
  parser.line_count = sizeof(lines) / sizeof(lines[0]);
  ParserLineBasicSetUp(&lines[0], "value", "key", subscripts[0], 1, "subkey");
  ParserLineBasicSetUp(&lines[1], "value", "key", subscripts[1], 0);
  SemanticAnalyzer_Init(&analyzer, &parser);

  actual = SemanticAnalyzer_LoadLines(&analyzer, &parser);

  assert(!actual);

  SemanticAnalyzer_Deinit(&analyzer);
}

static void LoadLines_SameKeyDifferentValueConstTypes_ResolveDifferences(void) {
  int actual;
  struct SemanticAnalyzer analyzer;
  struct ParserLine lines[2];
  struct Subscript subscripts[2][1];
  struct ValueExpr* value_expr[2];
  struct Parser parser;

  /*
   * key[1]: 0x1
   * key[2]: 2
   */
  parser.lines = lines;
  parser.line_count = sizeof(lines) / sizeof(lines[0]);
  ParserLineBasicSetUp(&lines[0], "value", "key", subscripts[0], 1, "0x1");
  ParserLineBasicSetUp(&lines[1], "value", "key", subscripts[1], 1, "2");
  value_expr[0] = &lines[0].variant.assign_statement.value_expr;
  value_expr[0]->variant.as_constexpr.type = ConstExprType_kUnsignedInt;
  value_expr[1] = &lines[1].variant.assign_statement.value_expr;
  value_expr[1]->variant.as_constexpr.type = ConstExprType_kSignedInt;
  SemanticAnalyzer_Init(&analyzer, &parser);

  actual = SemanticAnalyzer_LoadLines(&analyzer, &parser);

  assert(actual);
  assert(
      analyzer.typing_table.typings[0].value_as_constexpr_type
          == ConstExprType_kString);

  SemanticAnalyzer_Deinit(&analyzer);
}

static void LoadLines_SameKeyDifferentValueTypes_ResolveDifferences(void) {
  int actual;
  struct SemanticAnalyzer analyzer;
  struct LexerLine lline;
  struct LexerString lstr[4];
  struct ParserLine lines[2];
  struct Subscript subscripts[2][1];
  struct ValueExpr* value_expr[2];
  struct ToggleExpr* toggle_expr;
  struct ConstExpr* const_exprs[2];
  struct Parser parser;

  LexerLineSetUp(
      &lline,
      lstr,
      sizeof(lstr) / sizeof(lstr[0]),
      "true", 1,
      ",", 1,
      " ", 0,
      "VK_A", 1);

  /*
   * key[1]: true, VK_A
   * key[2]: 2
   */
  parser.lines = lines;
  parser.line_count = sizeof(lines) / sizeof(lines[0]);
  ParserLineBasicSetUp(&lines[0], "", "key", subscripts[0], 1, "1");
  ParserLineBasicSetUp(&lines[1], "2", "key", subscripts[1], 1, "2");
  value_expr[0] = &lines[0].variant.assign_statement.value_expr;
  value_expr[0]->type = ValueExprType_kToggle;
  toggle_expr = &value_expr[0]->variant.as_toggleexpr;
  /* Set up enabled. */
  toggle_expr->enabled_expr.type = ConstExprType_kBoolean;
  toggle_expr->enabled_expr.expr = malloc(sizeof("true"));
  memcpy(toggle_expr->enabled_expr.expr, "true", sizeof("true"));
  toggle_expr->enabled_expr.length = 4;
  toggle_expr->enabled_expr.begin_src = &lstr[0];
  toggle_expr->enabled_expr.end_src = &lstr[1];
  /* Set up input. */
  toggle_expr->input_expr.type = ConstExprType_kString;
  toggle_expr->input_expr.expr = malloc(sizeof("VK_A"));
  memcpy(toggle_expr->input_expr.expr, "VK_A", sizeof("VK_A"));
  toggle_expr->input_expr.length = 4;
  toggle_expr->input_expr.begin_src = &lstr[3];
  toggle_expr->input_expr.end_src = &lstr[4];
  /* Set up second line. */
  value_expr[1] = &lines[1].variant.assign_statement.value_expr;
  value_expr[1]->variant.as_constexpr.type = ConstExprType_kSignedInt;
  SemanticAnalyzer_Init(&analyzer, &parser);

  actual = SemanticAnalyzer_LoadLines(&analyzer, &parser);

  assert(actual);
  assert(
      analyzer.typing_table.typings[0].value_type == ValueExprType_kConst);
  assert(
      analyzer.typing_table.typings[0].value_as_constexpr_type
          == ConstExprType_kString);

  SemanticAnalyzer_Deinit(&analyzer);
}

static void LoadLines_SameKeyDifferentSubkeyTypes_ResolveDifferences(void) {
  int actual;
  struct SemanticAnalyzer analyzer;
  struct ParserLine lines[2];
  struct Subscript subscripts[2][1];
  struct KeyExpr* key_expr[2];
  struct Parser parser;

  /*
   * key[subkey]: value
   * key[2]: value
   */
  parser.lines = lines;
  parser.line_count = sizeof(lines) / sizeof(lines[0]);
  ParserLineBasicSetUp(&lines[0], "value", "key", subscripts[0], 1, "subkey");
  ParserLineBasicSetUp(&lines[1], "value", "key", subscripts[1], 1, "2");
  key_expr[0] = &lines[0].variant.assign_statement.key_expr;
  key_expr[0]->subscripts[0].expr.type = ConstExprType_kString;
  key_expr[1] = &lines[1].variant.assign_statement.key_expr;
  key_expr[1]->subscripts[0].expr.type = ConstExprType_kSignedInt;
  SemanticAnalyzer_Init(&analyzer, &parser);

  actual = SemanticAnalyzer_LoadLines(&analyzer, &parser);

  assert(actual);
  assert(
      analyzer.typing_table.typings[0].subkey_types[0]
          == ConstExprType_kString);

  SemanticAnalyzer_Deinit(&analyzer);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &LoadLines_TwoKeys_Success,
    &LoadLines_DuplicateNames_Failure,
    &LoadLines_DuplicateSubkeys_Failure,
    &LoadLines_SameKeyMismatch_Failure,
    &LoadLines_SameKeyDifferentValueConstTypes_ResolveDifferences,
    &LoadLines_SameKeyDifferentValueTypes_ResolveDifferences,
    &LoadLines_SameKeyDifferentSubkeyTypes_ResolveDifferences
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
