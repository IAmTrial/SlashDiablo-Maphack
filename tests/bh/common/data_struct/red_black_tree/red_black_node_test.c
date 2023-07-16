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

static void RotateAwayFromChild_AwayFromLeftChild_RotatedRight(
    struct EachContext* context) {
  RedBlackNode_SetLeft(&context->ten_node, &context->one_node);
  RedBlackNode_SetRight(&context->one_node, &context->five_node);
  RedBlackNode_SetRight(&context->ten_node, &context->twenty_node);

  RedBlackNode_RotateAwayFromChild(&context->ten_node, &context->one_node);

  assert(RedBlackNode_IsRoot(&context->one_node));
  assert(context->one_node.left == NULL);
  assert(context->one_node.right == &context->ten_node);
  assert(context->ten_node.left == &context->five_node);
  assert(context->ten_node.right == &context->twenty_node);
}

static void RotateAwayFromChild_AwayFromRightChild_RotatedLeft(
    struct EachContext* context) {
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->twenty_node);
  RedBlackNode_SetLeft(&context->twenty_node, &context->ten_node);

  RedBlackNode_RotateAwayFromChild(&context->five_node, &context->twenty_node);

  assert(RedBlackNode_IsRoot(&context->twenty_node));
  assert(context->twenty_node.left == &context->five_node);
  assert(context->twenty_node.right == NULL);
  assert(context->five_node.left == &context->one_node);
  assert(context->five_node.right == &context->ten_node);
}

static void RotateTowardChild_TowardLeftChild_RotatedLeft(
    struct EachContext* context) {
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->twenty_node);
  RedBlackNode_SetLeft(&context->twenty_node, &context->ten_node);

  RedBlackNode_RotateTowardChild(&context->five_node, &context->one_node);

  assert(RedBlackNode_IsRoot(&context->twenty_node));
  assert(context->twenty_node.left == &context->five_node);
  assert(context->twenty_node.right == NULL);
  assert(context->five_node.left == &context->one_node);
  assert(context->five_node.right == &context->ten_node);
}

static void RotateTowardChild_TowardRightChild_RotatedRight(
    struct EachContext* context) {
  RedBlackNode_SetLeft(&context->ten_node, &context->one_node);
  RedBlackNode_SetRight(&context->one_node, &context->five_node);
  RedBlackNode_SetRight(&context->ten_node, &context->twenty_node);

  RedBlackNode_RotateTowardChild(&context->ten_node, &context->twenty_node);

  assert(RedBlackNode_IsRoot(&context->one_node));
  assert(context->one_node.left == NULL);
  assert(context->one_node.right == &context->ten_node);
  assert(context->ten_node.left == &context->five_node);
  assert(context->ten_node.right == &context->twenty_node);
}

static void RotateAwayFromSibling_AwayFromLeftSibling_RotatedRight(
    struct EachContext* context) {
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->twenty_node);
  RedBlackNode_SetLeft(&context->twenty_node, &context->ten_node);

  RedBlackNode_RotateAwayFromSibling(&context->twenty_node);

  assert(RedBlackNode_IsLeaf(&context->twenty_node));
  assert(context->twenty_node.parent == &context->ten_node);
  assert(context->ten_node.right == &context->twenty_node);
  assert(context->ten_node.parent == &context->five_node);
  assert(context->five_node.right == &context->ten_node);
}

static void RotateAwayFromSibling_AwayFromRightSibling_RotatedLeft(
    struct EachContext* context) {
  RedBlackNode_SetLeft(&context->ten_node, &context->one_node);
  RedBlackNode_SetRight(&context->one_node, &context->five_node);
  RedBlackNode_SetRight(&context->ten_node, &context->twenty_node);

  RedBlackNode_RotateAwayFromSibling(&context->one_node);

  assert(RedBlackNode_IsLeaf(&context->one_node));
  assert(context->one_node.parent == &context->five_node);
  assert(context->five_node.left == &context->one_node);
  assert(context->five_node.parent == &context->ten_node);
  assert(context->ten_node.left == &context->five_node);
}

static void RotateTowardSibling_TowardLeftSibling_RotatedLeft(
    struct EachContext* context) {
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);
  RedBlackNode_SetRight(&context->ten_node, &context->twenty_node);

  RedBlackNode_RotateTowardSibling(&context->ten_node);

  assert(RedBlackNode_IsLeaf(&context->ten_node));
  assert(context->ten_node.parent == &context->twenty_node);
  assert(context->twenty_node.left == &context->ten_node);
  assert(context->twenty_node.parent == &context->five_node);
  assert(context->five_node.right == &context->twenty_node);
}

static void RotateTowardSibling_TowardRightSibling_RotatedRight(
    struct EachContext* context) {
  RedBlackNode_SetLeft(&context->ten_node, &context->five_node);
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->ten_node, &context->twenty_node);

  RedBlackNode_RotateTowardSibling(&context->five_node);

  assert(RedBlackNode_IsLeaf(&context->five_node));
  assert(context->five_node.parent == &context->one_node);
  assert(context->one_node.right == &context->five_node);
  assert(context->one_node.parent == &context->ten_node);
  assert(context->ten_node.left == &context->one_node);
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

static void GetGrandparent_NoParent_GetsNull(struct EachContext* context) {
  struct RedBlackNode* grandparent;

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

static void GetUncle_NoGrandparent_GetsNull(struct EachContext* context) {
  struct RedBlackNode* uncle;

  RedBlackNode_SetLeft(&context->ten_node, &context->one_node);

  uncle = RedBlackNode_GetUncle(&context->one_node);

  assert(uncle == NULL);
}

static void GetUncle_NoParent_GetsNull(struct EachContext* context) {
  struct RedBlackNode* uncle;

  uncle = RedBlackNode_GetUncle(&context->one_node);

  assert(uncle == NULL);
}

static void GetSibling_LeftSiblingIsOne_GetsOne(struct EachContext* context) {
  struct RedBlackNode* sibling;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  sibling = RedBlackNode_GetSibling(&context->ten_node);

  assert(sibling == &context->one_node);
}

static void GetSibling_RightSiblingIsTen_GetsTen(
    struct EachContext* context) {
  struct RedBlackNode* sibling;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  sibling = RedBlackNode_GetSibling(&context->one_node);

  assert(sibling == &context->ten_node);
}

static void GetSibling_NoLeftSibling_GetsNull(struct EachContext* context) {
  struct RedBlackNode* sibling;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);

  sibling = RedBlackNode_GetSibling(&context->one_node);

  assert(sibling == NULL);
}

static void GetSibling_NoRightSibling_GetsNull(struct EachContext* context) {
  struct RedBlackNode* sibling;

  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  sibling = RedBlackNode_GetSibling(&context->ten_node);

  assert(sibling == NULL);
}

static void GetSibling_NoParent_GetsNull(struct EachContext* context) {
  struct RedBlackNode* sibling;

  sibling = RedBlackNode_GetSibling(&context->one_node);

  assert(sibling == NULL);
}

static void GetFarNephew_LeftNephewIsOne_GetsOne(struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetLeft(&context->ten_node, &context->five_node);
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->ten_node, &context->twenty_node);

  nephew = RedBlackNode_GetFarNephew(&context->twenty_node);

  assert(nephew == &context->one_node);
}

static void GetFarNephew_RightNephewIsTwenty_GetsTwenty(
    struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);
  RedBlackNode_SetRight(&context->ten_node, &context->twenty_node);

  nephew = RedBlackNode_GetFarNephew(&context->one_node);

  assert(nephew == &context->twenty_node);
}

static void GetFarNephew_NoLeftNephew_GetsNull(struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  nephew = RedBlackNode_GetFarNephew(&context->ten_node);

  assert(nephew == NULL);
}

static void GetFarNephew_NoRightNephew_GetsNull(struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  nephew = RedBlackNode_GetFarNephew(&context->one_node);

  assert(nephew == NULL);
}

static void GetFarNephew_NoLeftSibling_GetsNull(struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  nephew = RedBlackNode_GetFarNephew(&context->ten_node);

  assert(nephew == NULL);
}

static void GetFarNephew_NoRightSibling_GetsNull(struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);

  nephew = RedBlackNode_GetFarNephew(&context->one_node);

  assert(nephew == NULL);
}

static void GetFarNephew_NoParent_GetsNull(struct EachContext* context) {
  struct RedBlackNode* nephew;

  nephew = RedBlackNode_GetFarNephew(&context->one_node);

  assert(nephew == NULL);
}

static void GetNearNephew_LeftNephewIsFive_GetsFive(
    struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetLeft(&context->ten_node, &context->one_node);
  RedBlackNode_SetRight(&context->one_node, &context->five_node);
  RedBlackNode_SetRight(&context->ten_node, &context->twenty_node);

  nephew = RedBlackNode_GetNearNephew(&context->twenty_node);

  assert(nephew == &context->five_node);
}

static void GetNearNephew_RightNephewIsTen_GetsTen(
    struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->twenty_node);
  RedBlackNode_SetLeft(&context->twenty_node, &context->ten_node);

  nephew = RedBlackNode_GetNearNephew(&context->one_node);

  assert(nephew == &context->ten_node);
}

static void GetNearNephew_NoLeftNephew_GetsNull(struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  nephew = RedBlackNode_GetNearNephew(&context->ten_node);

  assert(nephew == NULL);
}

static void GetNearNephew_NoRightNephew_GetsNull(struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  nephew = RedBlackNode_GetNearNephew(&context->one_node);

  assert(nephew == NULL);
}

static void GetNearNephew_NoLeftSibling_GetsNull(struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  nephew = RedBlackNode_GetNearNephew(&context->ten_node);

  assert(nephew == NULL);
}

static void GetNearNephew_NoRightSibling_GetsNull(
    struct EachContext* context) {
  struct RedBlackNode* nephew;

  RedBlackNode_SetLeft(&context->five_node, &context->one_node);

  nephew = RedBlackNode_GetNearNephew(&context->one_node);

  assert(nephew == NULL);
}

static void GetNearNephew_NoParent_GetsNull(struct EachContext* context) {
  struct RedBlackNode* nephew;

  nephew = RedBlackNode_GetNearNephew(&context->one_node);

  assert(nephew == NULL);
}

static void GetInorderSuccessor_LeftSuccessorIsTen_GetsTen(
    struct EachContext* context) {
  struct RedBlackNode* successor;

  RedBlackNode_SetLeft(&context->twenty_node, &context->one_node);
  RedBlackNode_SetRight(&context->one_node, &context->ten_node);
  RedBlackNode_SetLeft(&context->ten_node, &context->five_node);

  successor = RedBlackNode_GetInOrderSuccessor(&context->twenty_node);

  assert(successor == &context->ten_node);
}

static void GetInorderSuccessor_RightSuccessorIsFive_GetsFive(
    struct EachContext* context) {
  struct RedBlackNode* successor;

  RedBlackNode_SetRight(&context->one_node, &context->twenty_node);
  RedBlackNode_SetLeft(&context->twenty_node, &context->five_node);
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  successor = RedBlackNode_GetInOrderSuccessor(&context->one_node);

  assert(successor == &context->five_node);
}

static void GetInorderSuccessor_None_GetsNull(struct EachContext* context) {
  struct RedBlackNode* successor;

  successor = RedBlackNode_GetInOrderSuccessor(&context->one_node);

  assert(successor == NULL);
}

static void AddBlack_IsRed_IsBlack(struct EachContext* context) {
  context->one_node.color = RedBlackColor_kRed;

  RedBlackNode_AddBlack(&context->one_node);

  assert(context->one_node.color == RedBlackColor_kBlack);
}

static void AddBlack_IsBlack_IsDoubleBlack(struct EachContext* context) {
  context->one_node.color = RedBlackColor_kBlack;

  RedBlackNode_AddBlack(&context->one_node);

  assert(context->one_node.color == RedBlackColor_kDoubleBlack);
}

static void IsBlack_Red_False(struct EachContext* context) {
  context->one_node.color = RedBlackColor_kRed;

  assert(!RedBlackNode_IsBlack(&context->one_node));
}

static void IsBlack_Black_True(struct EachContext* context) {
  context->one_node.color = RedBlackColor_kBlack;

  assert(RedBlackNode_IsBlack(&context->one_node));
}

static void IsBlack_DoubleBlack_False(struct EachContext* context) {
  context->one_node.color = RedBlackColor_kDoubleBlack;

  assert(!RedBlackNode_IsBlack(&context->one_node));
}

static void IsBlack_Null_True(struct EachContext* context) {
  assert(RedBlackNode_IsBlack(NULL));
}

static void IsLeaf_NoLeftRight_True(struct EachContext* context) {
  assert(RedBlackNode_IsLeaf(&context->five_node));
}

static void IsLeaf_HasLeft_False(struct EachContext* context) {
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);

  assert(!RedBlackNode_IsLeaf(&context->five_node));
}

static void IsLeaf_HasRight_False(struct EachContext* context) {
  RedBlackNode_SetRight(&context->five_node, &context->ten_node);

  assert(!RedBlackNode_IsLeaf(&context->five_node));
}

static void IsRed_Red_True(struct EachContext* context) {
  context->one_node.color = RedBlackColor_kRed;

  assert(RedBlackNode_IsRed(&context->one_node));
}

static void IsRed_Black_False(struct EachContext* context) {
  context->one_node.color = RedBlackColor_kBlack;

  assert(!RedBlackNode_IsRed(&context->one_node));
}

static void IsRed_DoubleBlack_False(struct EachContext* context) {
  context->one_node.color = RedBlackColor_kDoubleBlack;

  assert(!RedBlackNode_IsRed(&context->one_node));
}

static void IsRed_Null_False(struct EachContext* context) {
  assert(!RedBlackNode_IsRed(NULL));
}

static void IsRoot_NoParent_True(struct EachContext* context) {
  assert(RedBlackNode_IsRoot(&context->five_node));
}

static void IsRoot_HasParent_False(struct EachContext* context) {
  RedBlackNode_SetRight(&context->five_node, &context->one_node);

  assert(!RedBlackNode_IsRoot(&context->one_node));
}

static void Detach_HasParent_Disowned(struct EachContext* context) {
  RedBlackNode_SetLeft(&context->five_node, &context->one_node);

  RedBlackNode_Detach(&context->one_node);

  assert(context->one_node.parent == NULL);
  assert(context->one_node.next == NULL);
  assert(context->five_node.left == NULL);
  assert(context->five_node.previous == NULL);
}

static void Detach_NoParent_DoNothing(struct EachContext* context) {
  RedBlackNode_Detach(&context->one_node);
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

static void SwapColor_RedAndBlack_BlackAndRed(struct EachContext* context) {
  context->one_node.color = RedBlackColor_kRed;
  context->five_node.color = RedBlackColor_kBlack;

  RedBlackNode_SwapColor(&context->one_node, &context->five_node);

  assert(context->one_node.color == RedBlackColor_kBlack);
  assert(context->five_node.color == RedBlackColor_kRed);
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

    &RotateAwayFromChild_AwayFromLeftChild_RotatedRight,
    &RotateAwayFromChild_AwayFromRightChild_RotatedLeft,

    &RotateTowardChild_TowardLeftChild_RotatedLeft,
    &RotateTowardChild_TowardRightChild_RotatedRight,

    &RotateAwayFromSibling_AwayFromLeftSibling_RotatedRight,
    &RotateAwayFromSibling_AwayFromRightSibling_RotatedLeft,

    &RotateTowardSibling_TowardLeftSibling_RotatedLeft,
    &RotateTowardSibling_TowardRightSibling_RotatedRight,

    &GetGrandparent_GrandparentIsTen_GetsTen,
    &GetGrandparent_NoGrandparent_GetsNull,
    &GetGrandparent_NoParent_GetsNull,

    &GetUncle_UncleIsOne_GetsOne,
    &GetUncle_NoUncle_GetsNull,
    &GetUncle_NoGrandparent_GetsNull,
    &GetUncle_NoParent_GetsNull,

    &GetSibling_LeftSiblingIsOne_GetsOne,
    &GetSibling_RightSiblingIsTen_GetsTen,
    &GetSibling_NoLeftSibling_GetsNull,
    &GetSibling_NoRightSibling_GetsNull,
    &GetSibling_NoParent_GetsNull,

    &GetFarNephew_LeftNephewIsOne_GetsOne,
    &GetFarNephew_RightNephewIsTwenty_GetsTwenty,
    &GetFarNephew_NoLeftNephew_GetsNull,
    &GetFarNephew_NoRightNephew_GetsNull,
    &GetFarNephew_NoLeftSibling_GetsNull,
    &GetFarNephew_NoRightSibling_GetsNull,
    &GetFarNephew_NoParent_GetsNull,

    &GetNearNephew_LeftNephewIsFive_GetsFive,
    &GetNearNephew_RightNephewIsTen_GetsTen,
    &GetNearNephew_NoLeftNephew_GetsNull,
    &GetNearNephew_NoRightNephew_GetsNull,
    &GetNearNephew_NoLeftSibling_GetsNull,
    &GetNearNephew_NoRightSibling_GetsNull,
    &GetNearNephew_NoParent_GetsNull,

    &GetInorderSuccessor_LeftSuccessorIsTen_GetsTen,
    &GetInorderSuccessor_RightSuccessorIsFive_GetsFive,
    &GetInorderSuccessor_None_GetsNull,

    &AddBlack_IsRed_IsBlack,
    &AddBlack_IsBlack_IsDoubleBlack,

    &IsBlack_Red_False,
    &IsBlack_Black_True,
    &IsBlack_DoubleBlack_False,
    &IsBlack_Null_True,

    &IsLeaf_NoLeftRight_True,
    &IsLeaf_HasLeft_False,
    &IsLeaf_HasRight_False,

    &IsRed_Red_True,
    &IsRed_Black_False,
    &IsRed_DoubleBlack_False,
    &IsRed_Null_False,

    &IsRoot_NoParent_True,
    &IsRoot_HasParent_False,

    &Detach_HasParent_Disowned,
    &Detach_NoParent_DoNothing,

    &ComparePointerData_OneTen_Negative,
    &ComparePointerData_OneOne_Zero,
    &ComparePointerData_TenOne_Positive,

    &SwapColor_RedAndBlack_BlackAndRed
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
