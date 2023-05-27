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
#include <stdlib.h>

#include "bh/common/data_struct/red_black_tree/red_black_node.h"

/**
 * Before/after test set run
 */

static void BeforeAllSetUp(void) {}

static void AfterAllSetUp(void) {}

/**
 * Before/after each test run
 */

struct EachContext {
  struct RedBlackNode one_node;
  struct RedBlackNode five_node;
  struct RedBlackNode ten_node;
  struct RedBlackNode twenty_node;
};

static void BeforeEach(struct EachContext* context) {
  static int kOne = 1;
  static int kFive = 5;
  static int kTen = 10;
  static int kTwenty = 20;

  RedBlackNode_InitDefault(&context->one_node);
  context->one_node.ptr = &kOne;

  RedBlackNode_InitDefault(&context->five_node);
  context->five_node.ptr = &kFive;

  RedBlackNode_InitDefault(&context->ten_node);
  context->ten_node.ptr = &kTen;

  RedBlackNode_InitDefault(&context->twenty_node);
  context->twenty_node.ptr = &kTwenty;
}

static void AfterEach(struct EachContext* context) {
  RedBlackNode_Deinit(&context->twenty_node);
  RedBlackNode_Deinit(&context->ten_node);
  RedBlackNode_Deinit(&context->five_node);
  RedBlackNode_Deinit(&context->one_node);
}

/**
 * Helper functions
 */

static int CompareInt(const int* left, const int* right) {
  return *left - *right;
}

static int CompareIntAsVoid(const void* left, const void* right) {
  return CompareInt(left, right);
}

/**
 * Tests
 */

typedef void TestFunc(struct EachContext* context);

static void SetLeft_OneFive_OneIsLeft(struct EachContext* context) {
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);

  assert(RedBlackNode_IsRoot(&context->five_node));
  assert(RedBlackNode_IsLeft(&context->one_node));
  assert(context->one_node.parent == &context->five_node);
  assert(context->one_node.next == &context->five_node);
  assert(context->five_node.left == &context->one_node);
  assert(context->five_node.previous == &context->one_node);
}

static void SetRight_FiveTen_TenIsRight(struct EachContext* context) {
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  assert(RedBlackNode_IsRoot(&context->five_node));
  assert(RedBlackNode_IsRight(&context->ten_node));
  assert(context->ten_node.parent == &context->five_node);
  assert(context->ten_node.previous == &context->five_node);
  assert(context->five_node.right == &context->ten_node);
  assert(context->five_node.next == &context->ten_node);
}

static void SetLeftThenRight_BalancedOneFiveTen_InOrder(
    struct EachContext* context) {
  struct RedBlackNode* it;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  assert(RedBlackNode_IsRoot(&context->five_node));
  assert(context->five_node.left == &context->one_node);
  assert(context->one_node.parent == &context->five_node);
  assert(context->five_node.right == &context->ten_node);
  assert(context->ten_node.parent == &context->five_node);
  for (it = &context->one_node; it != &context->ten_node; it = it->next) {
    assert(CompareInt(it->ptr, it->next->ptr) < 0);
  }
}

static void SetLeft_LineOneFiveTen_InOrder(struct EachContext* context) {
  struct RedBlackNode* it;

  RedBlackNode_SetLeft(&context->ten_node, &context->five_node);
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);

  assert(RedBlackNode_IsRoot(&context->ten_node));
  assert(context->ten_node.left == &context->five_node);
  assert(context->five_node.parent == &context->ten_node);
  assert(context->five_node.left == &context->one_node);
  assert(context->one_node.parent == &context->five_node);
  for (it = &context->one_node; it != &context->ten_node; it = it->next) {
    assert(CompareInt(it->ptr, it->next->ptr) < 0);
  }
}

static void SetRight_LineTenFiveOne_InOrder(struct EachContext* context) {
  struct RedBlackNode* it;

  RedBlackNode_SetRight(&context->one_node, &context->five_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  assert(RedBlackNode_IsRoot(&context->one_node));
  assert(context->one_node.right == &context->five_node);
  assert(context->five_node.parent == &context->one_node);
  assert(context->five_node.right == &context->ten_node);
  assert(context->ten_node.parent == &context->five_node);
  for (it = &context->one_node; it != &context->ten_node; it = it->next) {
    assert(CompareInt(it->ptr, it->next->ptr) < 0);
  }
}

static void SetLeftThenRight_TriangleTenOneFive_InOrder(
    struct EachContext* context) {
  struct RedBlackNode* it;

  RedBlackNode_SetLeft(&context->ten_node, &context->one_node);
  RedBlackNode_SetRight(&context->one_node, &context->five_node);

  assert(RedBlackNode_IsRoot(&context->ten_node));
  assert(context->ten_node.left == &context->one_node);
  assert(context->one_node.parent == &context->ten_node);
  assert(context->one_node.right == &context->five_node);
  assert(context->five_node.parent == &context->one_node);
  for (it = &context->one_node; it != &context->ten_node; it = it->next) {
    assert(CompareInt(it->ptr, it->next->ptr) < 0);
  }
}

static void SetRightThenLeft_TriangleOneTenFive_InOrder(
    struct EachContext* context) {
  struct RedBlackNode* it;

  RedBlackNode_SetRight(&context->one_node, &context->ten_node);
  RedBlackNode_SetLeft(&context->ten_node, &context->five_node);

  assert(RedBlackNode_IsRoot(&context->one_node));
  assert(context->one_node.right == &context->ten_node);
  assert(context->ten_node.parent == &context->one_node);
  assert(context->ten_node.left == &context->five_node);
  assert(context->five_node.parent == &context->ten_node);
  for (it = &context->one_node; it != &context->ten_node; it = it->next) {
    assert(CompareInt(it->ptr, it->next->ptr) < 0);
  }
}

static void RotateLeft_FiveTen_FiveIsLeft(struct EachContext* context) {
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  RedBlackNode_RotateLeft(&context->five_node);

  assert(RedBlackNode_IsRoot(&context->ten_node));
  assert(RedBlackNode_IsLeft(&context->five_node));
  assert(context->five_node.parent == &context->ten_node);
  assert(context->ten_node.left == &context->five_node);
}

static void RotateRight_OneFive_FiveIsRight(struct EachContext* context) {
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);

  RedBlackNode_RotateRight(&context->five_node);

  assert(RedBlackNode_IsRoot(&context->one_node));
  assert(RedBlackNode_IsRight(&context->five_node));
  assert(context->five_node.parent == &context->one_node);
  assert(context->one_node.right == &context->five_node);
}

static void RotateLeft_BalancedOneFiveTen_LineInOrder(
    struct EachContext* context) {
  struct RedBlackNode* it;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  RedBlackNode_RotateLeft(&context->five_node);

  assert(RedBlackNode_IsRoot(&context->ten_node));
  assert(context->ten_node.left == &context->five_node);
  assert(context->five_node.parent == &context->ten_node);
  assert(context->five_node.left == &context->one_node);
  assert(context->one_node.parent == &context->five_node);
  for (it = &context->one_node; it != &context->ten_node; it = it->next) {
    assert(CompareInt(it->ptr, it->next->ptr) < 0);
  }
}

static void RotateRight_BalancedOneFiveTen_LineInOrder(
    struct EachContext* context) {
  struct RedBlackNode* it;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  RedBlackNode_RotateRight(&context->five_node);

  assert(RedBlackNode_IsRoot(&context->one_node));
  assert(context->one_node.right == &context->five_node);
  assert(context->five_node.parent == &context->one_node);
  assert(context->five_node.right == &context->ten_node);
  assert(context->ten_node.parent == &context->five_node);
  for (it = &context->one_node; it != &context->ten_node; it = it->next) {
    assert(CompareInt(it->ptr, it->next->ptr) < 0);
  }
}

static void RotateLeft_LineOneFiveTen_BalancedInOrder(
    struct EachContext* context) {
  struct RedBlackNode* it;

  RedBlackNode_SetRight(&context->one_node, &context->five_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  RedBlackNode_RotateLeft(&context->one_node);

  assert(RedBlackNode_IsRoot(&context->five_node));
  assert(context->five_node.left == &context->one_node);
  assert(context->one_node.parent == &context->five_node);
  assert(context->five_node.right == &context->ten_node);
  assert(context->ten_node.parent == &context->five_node);
  for (it = &context->one_node; it != &context->ten_node; it = it->next) {
    assert(CompareInt(it->ptr, it->next->ptr) < 0);
  }
}

static void RotateRight_LineTenFiveOne_BalancedInOrder(
    struct EachContext* context) {
  struct RedBlackNode* it;

  RedBlackNode_SetLeft(&context->ten_node, &context->five_node);
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);

  RedBlackNode_RotateRight(&context->ten_node);

  assert(RedBlackNode_IsRoot(&context->five_node));
  assert(context->five_node.left == &context->one_node);
  assert(context->one_node.parent == &context->five_node);
  assert(context->five_node.right == &context->ten_node);
  assert(context->ten_node.parent == &context->five_node);
  for (it = &context->one_node; it != &context->ten_node; it = it->next) {
    assert(CompareInt(it->ptr, it->next->ptr) < 0);
  }
}

static void RotateLeft_TriangleOneTenFive_TriangleInOrder(
    struct EachContext* context) {
  struct RedBlackNode* it;

  RedBlackNode_SetRight(&context->one_node, &context->ten_node);
  RedBlackNode_SetLeft(&context->ten_node, &context->five_node);

  RedBlackNode_RotateLeft(&context->one_node);

  assert(RedBlackNode_IsRoot(&context->ten_node));
  assert(context->ten_node.left == &context->one_node);
  assert(context->one_node.parent == &context->ten_node);
  assert(context->one_node.right == &context->five_node);
  assert(context->five_node.parent == &context->one_node);
  for (it = &context->one_node; it != &context->ten_node; it = it->next) {
    assert(CompareInt(it->ptr, it->next->ptr) < 0);
  }
}

static void RotateRight_TriangleTenOneFive_TriangleInOrder(
    struct EachContext* context) {
  struct RedBlackNode* it;

  RedBlackNode_SetLeft(&context->ten_node, &context->one_node);
  RedBlackNode_SetRight(&context->one_node, &context->five_node);

  RedBlackNode_RotateRight(&context->ten_node);

  assert(RedBlackNode_IsRoot(&context->one_node));
  assert(context->one_node.right == &context->ten_node);
  assert(context->ten_node.parent == &context->one_node);
  assert(context->ten_node.left == &context->five_node);
  assert(context->five_node.parent == &context->ten_node);
  for (it = &context->one_node; it != &context->ten_node; it = it->next) {
    assert(CompareInt(it->ptr, it->next->ptr) < 0);
  }
}

static void GetGrandparent_GrandparentIsTen_GetsTen(
    struct EachContext* context) {
  struct RedBlackNode* grandparent;

  RedBlackNode_SetLeft(&context->ten_node, &context->one_node);
  RedBlackNode_SetRight(&context->one_node, &context->five_node);

  grandparent = RedBlackNode_GetGrandparent(&context->five_node);

  assert(grandparent == &context->ten_node);
}

static void GetGrandparent_NoGrandparent_GetsNull(
    struct EachContext* context) {
  struct RedBlackNode* grandparent;

  RedBlackNode_SetLeft(&context->ten_node, &context->one_node);

  grandparent = RedBlackNode_GetGrandparent(&context->one_node);

  assert(grandparent == NULL);
}

static void GetUncle_UncleIsOne_GetsOne(struct EachContext* context) {
  struct RedBlackNode* uncle;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->twenty_node);
  RedBlackNode_SetLeft(&context->twenty_node, &context->ten_node);

  uncle = RedBlackNode_GetUncle(&context->ten_node);

  assert(uncle == &context->one_node);
}

static void GetUncle_NoUncle_GetsNull(struct EachContext* context) {
  struct RedBlackNode* uncle;

  RedBlackNode_SetRight(&context->five_node, &context->twenty_node);
  RedBlackNode_SetLeft(&context->twenty_node, &context->ten_node);

  uncle = RedBlackNode_GetUncle(&context->ten_node);

  assert(uncle == NULL);
}

static void GetUncle_NoGrandparent_GetsNull(
    struct EachContext* context) {
  struct RedBlackNode* uncle;

  RedBlackNode_SetLeft(&context->ten_node, &context->one_node);

  uncle = RedBlackNode_GetUncle(&context->one_node);

  assert(uncle == NULL);
}

static void ComparePointerData_OneTen_Negative(struct EachContext* context) {
  int cmp_result;

  cmp_result =
      RedBlackNode_ComparePointerData(
          &context->one_node, &context->ten_node, &CompareIntAsVoid);

  assert(cmp_result < 0);
}

static void ComparePointerData_OneOne_Zero(struct EachContext* context) {
  int cmp_result;

  cmp_result =
      RedBlackNode_ComparePointerData(
          &context->one_node, &context->one_node, &CompareIntAsVoid);

  assert(cmp_result == 0);
}

static void ComparePointerData_TenOne_Positive(struct EachContext* context) {
  int cmp_result;

  cmp_result =
      RedBlackNode_ComparePointerData(
          &context->ten_node, &context->one_node, &CompareIntAsVoid);

  assert(cmp_result > 0);
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &SetLeft_OneFive_OneIsLeft,
    &SetRight_FiveTen_TenIsRight,

    &SetLeftThenRight_BalancedOneFiveTen_InOrder,
    &SetLeft_LineOneFiveTen_InOrder,
    &SetRight_LineTenFiveOne_InOrder,
    &SetLeftThenRight_TriangleTenOneFive_InOrder,
    &SetRightThenLeft_TriangleOneTenFive_InOrder,

    &RotateLeft_FiveTen_FiveIsLeft,
    &RotateRight_OneFive_FiveIsRight,

    &RotateLeft_BalancedOneFiveTen_LineInOrder,
    &RotateRight_BalancedOneFiveTen_LineInOrder,
    &RotateLeft_LineOneFiveTen_BalancedInOrder,
    &RotateRight_LineTenFiveOne_BalancedInOrder,
    &RotateLeft_TriangleOneTenFive_TriangleInOrder,
    &RotateRight_TriangleTenOneFive_TriangleInOrder,

    &GetGrandparent_GrandparentIsTen_GetsTen,
    &GetGrandparent_NoGrandparent_GetsNull,

    &GetUncle_UncleIsOne_GetsOne,
    &GetUncle_NoUncle_GetsNull,
    &GetUncle_NoGrandparent_GetsNull,

    &ComparePointerData_OneTen_Negative,
    &ComparePointerData_OneOne_Zero,
    &ComparePointerData_TenOne_Positive
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

  AfterAllSetUp();

#endif

  return 0;
}
