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

#include "bh/common/data_struct/red_black_tree.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "bh/common/data_struct/red_black_tree/red_black_node.h"

static struct RedBlackNode* RedBlackTree_FindParentNode(
    const struct RedBlackTree* tree,
    const void* data,
    RedBlackNode_CompareFuncType* compare_func) {
  struct RedBlackNode* current;

  if (tree->root == NULL) {
    return NULL;
  }

  for (current = tree->root; ; ) {
    int cmp_result;

    cmp_result = compare_func(data, current->ptr);
    if (cmp_result == 0) {
      return current->parent;
    }

    if (cmp_result < 0) {
      if (current->left == NULL) {
        return current;
      } else {
        current = current->left;
      }
    } else {
      assert(cmp_result > 0);

      if (current->right == NULL) {
        return current;
      } else {
        current = current->right;
      }
    }
  }
}

static struct RedBlackNode** RedBlackTree_FindNodeRef(
    const struct RedBlackTree* tree,
    const void* data,
    RedBlackNode_CompareFuncType* compare_func) {
  struct RedBlackNode* parent;
  int cmp_result;

  parent = RedBlackTree_FindParentNode(tree, data, compare_func);
  if (parent == NULL) {
    return (struct RedBlackNode**)&tree->root;
  }

  cmp_result = compare_func(data, parent->ptr);
  if (cmp_result < 0) {
    return &parent->left;
  } else {
    assert(cmp_result > 0);
    return &parent->right;
  }
}

/**
 * External
 */

struct RedBlackTree* RedBlackTree_Init(struct RedBlackTree* tree) {
  tree->count = 0;
  tree->first = NULL;
  tree->last = NULL;
  tree->root = NULL;

  return tree;
}

void RedBlackTree_Deinit(struct RedBlackTree* tree) {
  struct RedBlackNode* it;
  struct RedBlackNode* next;

  for (it = tree->first; it != NULL; it = next) {
    next = it->next;
    free(it);
  }
  tree->root = NULL;
  tree->last = NULL;
  tree->first = NULL;
  tree->count = 0;
}

int RedBlackTree_Compare(
    const struct RedBlackTree* left,
    const struct RedBlackTree* right,
    RedBlackNode_CompareFuncType* compare_func) {
  const struct RedBlackNode* it_l;
  const struct RedBlackNode* it_r;

  assert(left != NULL);
  assert(right != NULL);

  for (it_l = left->first, it_r = right->first;
      it_l != NULL && it_r != NULL;
      it_l = it_l->next, it_r = it_r->next) {
    int cmp_result;

    cmp_result = RedBlackNode_ComparePointerData(it_l, it_r, compare_func);
    if (cmp_result != 0) {
      return cmp_result;
    }
  }

  if (left->count < right->count) {
    assert(it_l == NULL);
    return -1;
  }

  assert(it_r == NULL);
  if (left->count > right->count) {
    return 1;
  }

  assert(left->count == right->count);
  assert(it_l == NULL);

  return 0;
}

int RedBlackTree_Contains(
    struct RedBlackTree* tree,
    const void* data,
    RedBlackNode_CompareFuncType* compare_func) {
  return (RedBlackTree_Find(tree, data, compare_func) != NULL);
}

int RedBlackTree_Insert(
    struct RedBlackTree* tree,
    void* data,
    RedBlackNode_CompareFuncType* compare_func) {
  struct RedBlackNode* new_node;
  struct RedBlackNode* parent_node;
  int cmp_result;

  struct RedBlackNode* current;
  struct RedBlackNode* parent;
  struct RedBlackNode* grandparent;

  /* Allocate space for a new node. */
  new_node = malloc(sizeof(*new_node));
  if (new_node == NULL) {
    goto error;
  }
  RedBlackNode_InitDefault(new_node);
  new_node->ptr = data;
  new_node->color = RedBlackColor_kRed;

  /* Add the new node to the matching position in the tree. */
  if (tree->root == NULL) {
    tree->root = new_node;
    tree->first = new_node;
    tree->last = new_node;
    tree->count = 1;
  } else {
    parent_node = RedBlackTree_FindParentNode(tree, data, compare_func);
    /* Insertion should be root, but root is already set. */
    if (parent_node == NULL) {
      return 0;
    }

    cmp_result =
        RedBlackNode_ComparePointerData(new_node, parent_node, compare_func);
    if (cmp_result < 0 && parent_node->left == NULL) {
      RedBlackNode_SetLeft(parent_node, new_node);
      if (tree->first == parent_node) {
        tree->first = new_node;
      }
    } else if (cmp_result > 0 && parent_node->right == NULL) {
      RedBlackNode_SetRight(parent_node, new_node);
      if (tree->last == parent_node) {
        tree->last = new_node;
      }
    } else {
      /* The node cannot be inserted because it would be a duplicate. */
      assert(cmp_result != 0);
      return 0;
    }

    ++tree->count;
  }

  /* Fix tree. */
  for (current = new_node; current != NULL; ) {
    struct RedBlackNode* uncle;

    /* Case 1: Current is Root. */
    if (RedBlackNode_IsRoot(current)) {
      current->color = RedBlackColor_kBlack;
      return 1;
    }

    /* Case 2: Parent is colored black. */
    parent = current->parent;
    if (parent->color == RedBlackColor_kBlack) {
      return 1;
    }
    assert(parent->color == RedBlackColor_kRed);

    /* Case 3: Parent is Root. */
    if (RedBlackNode_IsRoot(parent)) {
      current->color = RedBlackColor_kBlack;
      return 1;
    }

    grandparent = RedBlackNode_GetGrandparent(current);
    assert(grandparent != NULL);

    /* Case 4: Parent and Uncle are colored red. */
    uncle = RedBlackNode_GetUncle(current);
    if (uncle != NULL && uncle->color == RedBlackColor_kRed) {
      parent->color = RedBlackColor_kBlack;
      uncle->color = RedBlackColor_kBlack;
      grandparent->color = RedBlackColor_kRed;
      current = grandparent;
      continue;
    }

    /* Current and Parent are colored red, so the loop is finished. */
    break;
  }

  /* Case 5: Current's value is in between parent and grandparent. */
  if (RedBlackNode_IsRight(current) && RedBlackNode_IsLeft(parent)) {
    RedBlackNode_RotateLeft(parent);
    current = parent;
  } else if (RedBlackNode_IsLeft(current) && RedBlackNode_IsRight(parent)) {
    RedBlackNode_RotateRight(parent);
    current = parent;
  }
  parent = current->parent;
  /* Fall-through is intentional. */

  /* Case 6: Current's value is less/greater than parent and grandparent. */
  RedBlackNode_RotateAwayFromChild(grandparent, parent);

  grandparent->color = RedBlackColor_kRed;
  parent->color = RedBlackColor_kBlack;

  if (tree->root == grandparent) {
    tree->root = parent;
  }

  return 1;

error:
  return 0;
}

int RedBlackTree_IsEmpty(const struct RedBlackTree* tree) {
  return (tree->count == 0);
}

struct RedBlackNode* RedBlackTree_Find(
    const struct RedBlackTree* tree,
    const void* data,
    RedBlackNode_CompareFuncType* compare_func) {
  return *RedBlackTree_FindNodeRef(tree, data, compare_func);
}

void RedBlackTree_Remove(
    struct RedBlackTree* tree,
    const void* data,
    RedBlackNode_CompareFuncType* compare_func) {
  struct RedBlackNode* target_node;
  struct RedBlackNode* successor;
  struct RedBlackNode* current_node;

  target_node = RedBlackTree_Find(tree, data, compare_func);
  if (target_node == NULL) {
    return;
  }

  /*
   * Only leaf nodes are deleted from the tree. Replace non-leaf with
   * data from the in-order successor. This will override the data
   * in the first target node.
   */
  for (; !RedBlackNode_IsLeaf(target_node); target_node = successor) {
    successor = RedBlackNode_GetInOrderSuccessor(target_node);
    target_node->ptr = successor->ptr;
  }
  --tree->count;
  RedBlackNode_AddBlack(target_node);

  /* Case 1: Target node is root. */
  if (RedBlackNode_IsRoot(target_node)) {
    tree->first = NULL;
    tree->last = NULL;
    tree->root = NULL;
  }

  /* Case 2: If target node is red, loop won't run. */
  for (current_node = target_node;
      current_node->color == RedBlackColor_kDoubleBlack; ) {
    struct RedBlackNode* parent;
    struct RedBlackNode* sibling;
    struct RedBlackNode* near_nephew;
    struct RedBlackNode* far_nephew;

    /* Current node is now double-black, so fixes must be applied. */

    /* Case 1: Current node is root. */
    if (RedBlackNode_IsRoot(current_node)) {
      current_node->color = RedBlackColor_kBlack;
      break;
    }
    parent = current_node->parent;
    assert(parent != NULL);

    /* Case 3: Sibling is red. */
    sibling = RedBlackNode_GetSibling(current_node);
    assert(sibling != NULL);
    if (sibling->color == RedBlackColor_kRed) {
      RedBlackNode_SwapColor(sibling, parent);
      RedBlackNode_RotateTowardChild(parent, current_node);
      if (tree->root == parent) {
        tree->root = sibling;
      }
      continue;
    }
    assert(sibling->color == RedBlackColor_kBlack);

    /* Case 4: Sibling is black, far nephew is red. */
    far_nephew = RedBlackNode_GetFarNephew(current_node);
    if (RedBlackNode_IsRed(far_nephew)) {
      RedBlackNode_SwapColor(parent, sibling);
      RedBlackNode_RotateTowardChild(parent, current_node);
      far_nephew->color = RedBlackColor_kBlack;
      current_node->color = RedBlackColor_kBlack;
      if (tree->root == parent) {
        tree->root = sibling;
      }
      break;
    }
    assert(RedBlackNode_IsBlack(far_nephew));

    /* Case 5: Sibling is black, far nephew is black, near nephew is red. */
    near_nephew = RedBlackNode_GetNearNephew(current_node);
    if (RedBlackNode_IsRed(near_nephew)) {
      RedBlackNode_SwapColor(near_nephew, sibling);
      RedBlackNode_RotateAwayFromSibling(sibling);
      continue;
    }
    assert(RedBlackNode_IsBlack(near_nephew));

    /* Case 6: Sibling is black, both nephews are black. */
    RedBlackNode_AddBlack(parent);
    current_node->color = RedBlackColor_kBlack;
    sibling->color = RedBlackColor_kRed;

    current_node = parent;
  }

  if (target_node == tree->first) {
    tree->first = target_node->next;
  } else if (target_node == tree->last) {
    tree->last = target_node->previous;
  }
  RedBlackNode_Detach(target_node);
  RedBlackNode_Deinit(target_node);
  free(target_node);
}
