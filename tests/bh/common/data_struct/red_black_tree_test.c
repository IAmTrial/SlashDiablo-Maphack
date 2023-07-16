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
#include <string.h>
#include <time.h>

#include "bh/common/data_struct/red_black_tree.h"
#include "bh/common/data_struct/red_black_tree/red_black_color.h"
#include "bh/common/data_struct/red_black_tree/red_black_node.h"

static int k0To4096[4096];
static int kOne = 1;
static int kFive = 5;
static int kRandom[4096];
static int kZeroToTen[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
enum {
  k0To4096Count = sizeof(k0To4096) / sizeof(k0To4096[0]),
  kRandomCount = sizeof(kRandom) / sizeof(kRandom[0]),
  kZeroToTenCount = sizeof(kZeroToTen) / sizeof(kZeroToTen[0])
};

/**
 * Before/after test set run
 */

static void BeforeAllSetUp(void) {
  size_t i;
  time_t current_time;
  struct tm current_tm;

  current_time = time(NULL);
  current_tm = *localtime(&current_time);

  srand(
      current_tm.tm_sec * 100
          + current_tm.tm_min * 60 * 100
          + current_tm.tm_hour * 3600 * 100);

  for (i = 0; i < k0To4096Count; ++i) {
    k0To4096[i] = i;
  }

  for (i = 0; i < kRandomCount; ++i) {
    kRandom[i] = rand();
  }
}

static void AfterAllSetUp(void) {}

/**
 * Before/after each test run
 */

struct EachContext {
  struct RedBlackTree tree;
  struct RedBlackTree tree2;
};

static void BeforeEach(struct EachContext* context) {
  struct RedBlackTree* init_result;

  init_result = RedBlackTree_Init(&context->tree);
  assert(init_result == &context->tree);
  init_result = RedBlackTree_Init(&context->tree2);
  assert(init_result == &context->tree2);
}

static void AfterEach(struct EachContext* context) {
  RedBlackTree_Deinit(&context->tree2);
  RedBlackTree_Deinit(&context->tree);
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

static int CompareStringAsVoid(const void* left, const void* right) {
  return strcmp(left, right);
}

/**
 * Tests
 */

typedef void TestFunc(struct EachContext* context);

static void Insert_One_RootColoredBlack(struct EachContext* context) {
  int insert_result;
  struct RedBlackNode* node;

  insert_result =
      RedBlackTree_Insert(&context->tree, &kOne, &CompareIntAsVoid);

  assert(insert_result);
  assert(RedBlackTree_Contains(&context->tree, &kOne, &CompareIntAsVoid));
  assert(context->tree.count == 1);
  assert(context->tree.first == context->tree.last);
  node = RedBlackTree_Find(&context->tree, &kOne, &CompareIntAsVoid);
  assert(context->tree.root == node);
  assert(context->tree.first == node);
  assert(node->color == RedBlackColor_kBlack);
  assert(RedBlackNode_IsRoot(node));
}

static void Insert_OneFive_InOrder(struct EachContext* context) {
  int insert_one_result;
  int insert_five_result;
  struct RedBlackNode* node_one;
  struct RedBlackNode* node_five;

  insert_one_result =
      RedBlackTree_Insert(&context->tree, &kOne, &CompareIntAsVoid);
  insert_five_result =
      RedBlackTree_Insert(&context->tree, &kFive, &CompareIntAsVoid);

  assert(insert_one_result);
  assert(insert_five_result);
  assert(context->tree.count == 2);
  assert(RedBlackTree_Contains(&context->tree, &kOne, &CompareIntAsVoid));
  node_one = RedBlackTree_Find(&context->tree, &kOne, &CompareIntAsVoid);
  assert(node_one == context->tree.first);
  assert(node_one == context->tree.root);
  assert(RedBlackNode_IsRoot(node_one));
  assert(node_one->color == RedBlackColor_kBlack);
  assert(RedBlackTree_Contains(&context->tree, &kFive, &CompareIntAsVoid));
  node_five = RedBlackTree_Find(&context->tree, &kFive, &CompareIntAsVoid);
  assert(node_one->right == node_five);
  assert(node_five == context->tree.last);
  assert(node_five->parent == node_one);
  assert(node_five->color == RedBlackColor_kRed);
}

static void Insert_ZeroToTen_InOrder(struct EachContext* context) {
  size_t i;
  struct RedBlackNode* it;

  for (i = 0; i < kZeroToTenCount; ++i) {
    int insert_result;

    insert_result =
        RedBlackTree_Insert(&context->tree, &kZeroToTen[i], &CompareIntAsVoid);
    assert(insert_result);
  }

  for (it = context->tree.first, i = 0; it != NULL; it = it->next, ++i) {
    const int* data_as_int;

    data_as_int = it->ptr;
    assert(kZeroToTen[i] == *data_as_int);
  }
}

static void Insert_Random4096_InOrder(
    struct EachContext* context) {
  size_t i;
  struct RedBlackNode* it;

  for (i = 0; i < kRandomCount; ++i) {
    int insert_result;

    insert_result =
        RedBlackTree_Insert(&context->tree, &kRandom[i], &CompareIntAsVoid);
  }

  for (it = context->tree.first; it != context->tree.last; it = it->next) {
    const int* data_as_int;
    const int* next_data_as_int;

    data_as_int = it->ptr;
    next_data_as_int = it->next->ptr;

    assert(*data_as_int < *next_data_as_int);
  }
}

static void Insert_DuplicateRandom4096_NoDuplicates(
    struct EachContext* context) {
  size_t i;
  struct RedBlackNode* it;

  for (i = 0; i < kRandomCount; ++i) {
    int insert_result;

    insert_result =
        RedBlackTree_Insert(&context->tree, &kRandom[i], &CompareIntAsVoid);
  }

  for (i = 0; i < kRandomCount; ++i) {
    int insert_result;

    insert_result =
        RedBlackTree_Insert(&context->tree, &kRandom[i], &CompareIntAsVoid);
    assert(!insert_result);
  }
}

static void Insert_Random4096_Contains(
    struct EachContext* context) {
  size_t i;

  for (i = 0; i < kRandomCount; ++i) {
    int insert_result;

    insert_result =
        RedBlackTree_Insert(&context->tree, &kRandom[i], &CompareIntAsVoid);
  }

  for (i = 0; i < kRandomCount; ++i) {
    assert(
        RedBlackTree_Contains(
            &context->tree,
            &kRandom[i],
            &CompareIntAsVoid));
  }
}

static void Find_Empty_ReturnsNull(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kRandomCount; ++i) {
    struct RedBlackNode* find_result;

    find_result =
        RedBlackTree_Find(&context->tree, &kRandom[i], &CompareIntAsVoid);
    assert(find_result == NULL);
  }
}

static void Contains_Empty_NotContains(struct EachContext* context) {
  size_t i;

  for (i = 0; i < kRandomCount; ++i) {
    int random;

    random = rand();
    assert(
        !RedBlackTree_Contains(
            &context->tree, &kRandom[i], &CompareIntAsVoid));
  }
}

static void Contains_InsertFirst4096_NotContainsNext4096(
    struct EachContext* context) {  
  size_t i;

  for (i = 0; i < k0To4096Count; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }

  for (i = 0; i < k0To4096Count; ++i) {
    int plus_4096;

    plus_4096 = i + 4096;
    assert(
        !RedBlackTree_Contains(
            &context->tree, &plus_4096, &CompareIntAsVoid));
  }
}

static void Compare_Equal_Zero(struct EachContext* context) {
  size_t i;
  int cmp_result;

  for (i = 0; i < k0To4096Count; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
    RedBlackTree_Insert(
        &context->tree2,
        &k0To4096[(k0To4096Count - 1) - i],
        &CompareIntAsVoid);
  }

  cmp_result =
      RedBlackTree_Compare(
          &context->tree, &context->tree2, &CompareIntAsVoid);

  assert(cmp_result == 0);
}

static void Compare_LessThan_Negative(struct EachContext* context) {
  size_t i;
  int cmp_result;

  RedBlackTree_Insert(&context->tree, &k0To4096[0], &CompareIntAsVoid);
  for (i = 0; i < k0To4096Count - 1; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }
  for (i = 0; i < k0To4096Count; ++i) {
    RedBlackTree_Insert(&context->tree2, &k0To4096[i], &CompareIntAsVoid);
  }

  cmp_result =
      RedBlackTree_Compare(
          &context->tree, &context->tree2, &CompareIntAsVoid);

  assert(cmp_result < 0);
}

static void Compare_FewerCount_Negative(struct EachContext* context) {
  size_t i;
  int cmp_result;

  for (i = 0; i < k0To4096Count - 1; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }
  for (i = 0; i < k0To4096Count; ++i) {
    RedBlackTree_Insert(&context->tree2, &k0To4096[i], &CompareIntAsVoid);
  }

  cmp_result =
      RedBlackTree_Compare(
          &context->tree, &context->tree2, &CompareIntAsVoid);

  assert(cmp_result < 0);
}

static void Compare_GreaterThan_Positive(struct EachContext* context) {
  size_t i;
  int cmp_result;

  for (i = 0; i < k0To4096Count; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }
  RedBlackTree_Insert(&context->tree2, &k0To4096[0], &CompareIntAsVoid);
  for (i = 0; i < k0To4096Count - 1; ++i) {
    RedBlackTree_Insert(&context->tree2, &k0To4096[i], &CompareIntAsVoid);
  }

  cmp_result =
      RedBlackTree_Compare(
          &context->tree, &context->tree2, &CompareIntAsVoid);

  assert(cmp_result > 0);
}

static void Compare_MoreCount_Positive(struct EachContext* context) {
  size_t i;
  int cmp_result;

  for (i = 0; i < k0To4096Count; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }
  for (i = 0; i < k0To4096Count - 1; ++i) {
    RedBlackTree_Insert(&context->tree2, &k0To4096[i], &CompareIntAsVoid);
  }

  cmp_result =
      RedBlackTree_Compare(
          &context->tree, &context->tree2, &CompareIntAsVoid);

  assert(cmp_result > 0);
}

static void Remove_EmptyTree_Same(struct EachContext* context) {
  int kOne;

  kOne = 1;

  RedBlackTree_Remove(&context->tree, &kOne, &CompareIntAsVoid);

  assert(RedBlackTree_IsEmpty(&context->tree));
  assert(context->tree.first == NULL);
  assert(context->tree.last == NULL);
  assert(context->tree.root == NULL);
}

static void Remove_NotFound_Same(struct EachContext* context) {
  size_t i;
  int k4096;

  k4096 = 4096;
  for (i = 0; i < k0To4096Count; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }
  for (i = 0; i < k0To4096Count; ++i) {
    RedBlackTree_Insert(&context->tree2, &k0To4096[i], &CompareIntAsVoid);
  }

  RedBlackTree_Remove(&context->tree, &k4096, &CompareIntAsVoid);

  assert(context->tree.count == k0To4096Count);
  assert(
      RedBlackTree_Compare(
          &context->tree, &context->tree2, &CompareIntAsVoid) == 0);
}

static void Remove_RootValue_Success(struct EachContext* context) {
  size_t i;
  int* value;

  for (i = 0; i < k0To4096Count; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }
  value = context->tree.root->ptr;

  RedBlackTree_Remove(&context->tree, value, &CompareIntAsVoid);

  assert(context->tree.count == k0To4096Count - 1);
  assert(context->tree.root->ptr != value);
  assert(!RedBlackTree_Contains(&context->tree, value, &CompareIntAsVoid));
}

static void Remove_DeleteRoot_Success(struct EachContext* context) {
  RedBlackTree_Insert(&context->tree, &k0To4096[0], &CompareIntAsVoid);

  RedBlackTree_Remove(&context->tree, &k0To4096[0], &CompareIntAsVoid);

  assert(RedBlackTree_IsEmpty(&context->tree));
  assert(context->tree.root == NULL);
  assert(context->tree.first == NULL);
  assert(context->tree.last == NULL);
}

static void Remove_DeleteRed_Success(struct EachContext* context) {
  size_t i;
  struct RedBlackNode* current;

  for (i = 0; i < 4; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }

  RedBlackTree_Remove(&context->tree, &k0To4096[3], &CompareIntAsVoid);

  assert(context->tree.count == 3);
  for (current = context->tree.first;
      current != NULL;
      current = current->next) {
    assert(current->color == RedBlackColor_kBlack);
  }
}

static void Remove_DeleteBlackWithRedSibling_Success(
    struct EachContext* context) {
  size_t i;
  struct RedBlackNode* current;

  for (i = 0; i < 6; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }

  RedBlackTree_Remove(&context->tree, &k0To4096[0], &CompareIntAsVoid);
  
  assert(context->tree.count == 5);
  assert(CompareInt(context->tree.root->ptr, &k0To4096[3]) == 0);
}

static void Remove_DeleteBlackWithFarRedNephew_Success(
    struct EachContext* context) {
  size_t i;
  struct RedBlackNode* current;

  for (i = 0; i < 4; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }

  RedBlackTree_Remove(&context->tree, &k0To4096[0], &CompareIntAsVoid);
  
  assert(context->tree.count == 3);
  for (current = context->tree.first;
      current != NULL;
      current = current->next) {
    assert(current->color == RedBlackColor_kBlack);
  }
}

static void Remove_DeleteBlackWithNearRedNephew_Success(
    struct EachContext* context) {
  struct RedBlackNode* current;

  RedBlackTree_Insert(&context->tree, &k0To4096[1], &CompareIntAsVoid);
  RedBlackTree_Insert(&context->tree, &k0To4096[0], &CompareIntAsVoid);
  RedBlackTree_Insert(&context->tree, &k0To4096[3], &CompareIntAsVoid);
  RedBlackTree_Insert(&context->tree, &k0To4096[2], &CompareIntAsVoid);

  RedBlackTree_Remove(&context->tree, &k0To4096[0], &CompareIntAsVoid);

  assert(context->tree.count == 3);
  for (current = context->tree.first;
      current != NULL;
      current = current->next) {
    assert(current->color == RedBlackColor_kBlack);
  }
}

static void Remove_DeleteBlackWithBothRedNephew_Success(
    struct EachContext* context) {
  size_t i;
  struct RedBlackNode* current;

  for (i = 0; i < 5; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }

  RedBlackTree_Remove(&context->tree, &k0To4096[0], &CompareIntAsVoid);

  assert(context->tree.count == 4);
  for (current = context->tree.first;
      current != NULL;
      current = current->next) {
    int cmp_result;

    cmp_result = CompareInt(current->ptr, &k0To4096[2]);
    if (cmp_result == 0) {
      assert(current->color == RedBlackColor_kRed);
    } else {
      assert(current->color == RedBlackColor_kBlack);
    }
  }
}

static void Remove_DeleteBlackWithBothBlackNephews_Success(
    struct EachContext* context) {
  size_t i;
  struct RedBlackNode* current;

  for (i = 0; i < 6; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }
  RedBlackTree_Remove(&context->tree, &k0To4096[5], &CompareIntAsVoid);

  RedBlackTree_Remove(&context->tree, &k0To4096[4], &CompareIntAsVoid);

  assert(context->tree.count == 4);
  for (current = context->tree.first;
      current != NULL;
      current = current->next) {
    int cmp_result;

    cmp_result = CompareInt(current->ptr, &k0To4096[2]);
    if (cmp_result == 0) {
      assert(current->color == RedBlackColor_kRed);
    } else {
      assert(current->color == RedBlackColor_kBlack);
    }
  }
}

static void Remove_All_Empty(struct EachContext* context) {
  size_t i;

  for (i = 0; i < k0To4096Count; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }
  
  for (i = k0To4096Count; i-- > 0; ) {
    RedBlackTree_Remove(&context->tree, &k0To4096[i], &CompareIntAsVoid);
  }

  assert(RedBlackTree_IsEmpty(&context->tree));
}

static void Remove_AllInterleaved_Success(struct EachContext* context) {
  size_t i;

  for (i = 0; i < k0To4096Count; ++i) {
    RedBlackTree_Insert(&context->tree, &k0To4096[i], &CompareIntAsVoid);
    RedBlackTree_Remove(
        &context->tree, &k0To4096[k0To4096Count - 1 - i], &CompareIntAsVoid);
  }

  for (i = 0; i < k0To4096Count / 2; ++i) {
    assert(
        !RedBlackTree_Contains(
            &context->tree, &k0To4096[i], &CompareIntAsVoid));
  }
  for (i = k0To4096Count / 2; i < k0To4096Count; ++i) {
    assert(
        RedBlackTree_Contains(
            &context->tree, &k0To4096[i], &CompareIntAsVoid));
  }
}

int main(int argc, char** argv) {
#ifdef NDEBUG

  printf("Tests must run in debug mode, with NDEBUG not defined.\n");

#else

  static TestFunc* const kTests[] = {
    &Insert_One_RootColoredBlack,
    &Insert_OneFive_InOrder,

    &Insert_ZeroToTen_InOrder,
    &Insert_Random4096_InOrder,
    &Insert_DuplicateRandom4096_NoDuplicates,

    &Insert_Random4096_Contains,

    &Find_Empty_ReturnsNull,

    &Contains_Empty_NotContains,
    &Contains_InsertFirst4096_NotContainsNext4096,

    &Compare_Equal_Zero,
    &Compare_LessThan_Negative,
    &Compare_FewerCount_Negative,
    &Compare_GreaterThan_Positive,
    &Compare_MoreCount_Positive,

    &Remove_EmptyTree_Same,
    &Remove_NotFound_Same,
    &Remove_RootValue_Success,
    &Remove_DeleteRoot_Success,
    &Remove_DeleteRed_Success,
    &Remove_DeleteBlackWithRedSibling_Success,
    &Remove_DeleteBlackWithFarRedNephew_Success,
    &Remove_DeleteBlackWithNearRedNephew_Success,
    &Remove_DeleteBlackWithBothRedNephew_Success,
    &Remove_DeleteBlackWithBothBlackNephews_Success,
    &Remove_All_Empty,
    &Remove_AllInterleaved_Success
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
