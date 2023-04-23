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

static void Check_TwoKeys_Success(void) {
  int actual;
  struct ParserLine lines[2];
  struct Subscript subscripts[2][1];

  /*
   * key1: value
   * key2: value
   */
  ParserLineBasicSetUp(&lines[0], "value", "key1", subscripts[0], 0);
  ParserLineBasicSetUp(&lines[1], "value", "key2", subscripts[1], 0);

  actual = SemanticAnalyzer_Check(lines, 2);

  assert(actual);
}

static void Check_SameKeyMismatch_Failure(void) {
  int actual;
  struct ParserLine lines[2];
  struct Subscript subscripts[2][1];

  /*
   * key[subkey]: value
   * key: value
   */
  ParserLineBasicSetUp(&lines[0], "value", "key", subscripts[0], 1, "subkey");
  ParserLineBasicSetUp(&lines[1], "value", "key", subscripts[1], 0);

  actual = SemanticAnalyzer_Check(lines, 2);

  assert(!actual);
}

static void Check_SameKeyDifferentValueConstTypes_ResolveDifferences(void) {
  int actual;
  struct ParserLine lines[2];
  struct Subscript subscripts[2][1];
  struct ValueExpr* value_expr[2];

  /*
   * key[1]: 0x1
   * key[2]: 2
   */
  ParserLineBasicSetUp(&lines[0], "value", "key", subscripts[0], 1, "0x1");
  ParserLineBasicSetUp(&lines[1], "value", "key", subscripts[1], 1, "2");
  value_expr[0] = &lines[0].variant.assign_statement.value_expr;
  value_expr[0]->variant.as_constexpr.type = ConstExprType_kUnsignedInt;
  value_expr[1] = &lines[1].variant.assign_statement.value_expr;
  value_expr[1]->variant.as_constexpr.type = ConstExprType_kSignedInt;

  actual = SemanticAnalyzer_Check(lines, 2);

  assert(actual);
  assert(value_expr[0]->variant.as_constexpr.type == ConstExprType_kString);
  assert(value_expr[1]->variant.as_constexpr.type == ConstExprType_kString);
}

static void Check_SameKeyDifferentValueTypes_ResolveDifferences(void) {
  int actual;
  struct LexerLine lline;
  struct LexerString lstr[4];
  struct ParserLine lines[2];
  struct Subscript subscripts[2][1];
  struct ValueExpr* value_expr[2];
  struct ToggleExpr* toggle_expr;
  struct ConstExpr* const_exprs[2];

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

  actual = SemanticAnalyzer_Check(lines, 2);

  assert(actual);
  assert(value_expr[0]->type == ValueExprType_kConst);
  const_exprs[0] = &value_expr[0]->variant.as_constexpr;
  assert(const_exprs[0]->type == ConstExprType_kString);
  assert(const_exprs[0]->length == sizeof("true, VK_A") - 1);
  assert(
      memcmp(const_exprs[0]->expr, "true, VK_A", sizeof("true, VK_A")) == 0);
  const_exprs[1] = &value_expr[1]->variant.as_constexpr;
  assert(const_exprs[1]->type == ConstExprType_kString);
}

static void Check_SameKeyDifferentSubkeyTypes_ResolveDifferences(void) {
  int actual;
  struct ParserLine lines[2];
  struct Subscript subscripts[2][1];
  struct KeyExpr* key_expr[2];

  /*
   * key[subkey]: value
   * key[2]: value
   */
  ParserLineBasicSetUp(&lines[0], "value", "key", subscripts[0], 1, "subkey");
  ParserLineBasicSetUp(&lines[1], "value", "key", subscripts[1], 1, "2");
  key_expr[0] = &lines[0].variant.assign_statement.key_expr;
  key_expr[0]->subscripts[0].expr.type = ConstExprType_kString;
  key_expr[1] = &lines[1].variant.assign_statement.key_expr;
  key_expr[1]->subscripts[0].expr.type = ConstExprType_kSignedInt;

  actual = SemanticAnalyzer_Check(lines, 2);

  assert(actual);
  assert(key_expr[0]->subscripts[0].expr.type == ConstExprType_kString);
  assert(key_expr[1]->subscripts[0].expr.type == ConstExprType_kString);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &Check_TwoKeys_Success,
    &Check_SameKeyMismatch_Failure,
    &Check_SameKeyDifferentValueConstTypes_ResolveDifferences,
    &Check_SameKeyDifferentValueTypes_ResolveDifferences,
    &Check_SameKeyDifferentSubkeyTypes_ResolveDifferences
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
