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

#include "bh/common/data_struct/red_black_tree/red_black_node.h"

#include <assert.h>
#include <stddef.h>

#include "bh/common/data_struct/red_black_tree/red_black_color.h"

/**
 * External
 */

struct RedBlackNode* RedBlackNode_InitDefault(struct RedBlackNode* node) {
  node->color = RedBlackColor_kUnspecified;
  node->previous = NULL;
  node->next = NULL;
  node->parent = NULL;
  node->left = NULL;
  node->right = NULL;

  return node;
}

void RedBlackNode_Deinit(struct RedBlackNode* node) {}

void RedBlackNode_AddBlack(struct RedBlackNode* node) {
  switch (node->color) {
    case RedBlackColor_kRed: {
      node->color = RedBlackColor_kBlack;
      break;
    }

    case RedBlackColor_kBlack: {
      node->color = RedBlackColor_kDoubleBlack;
      break;
    }

    default: {
      assert(0 && "This should never happen.");
      break;
    }
  }
}

int RedBlackNode_ComparePointerData(
    const struct RedBlackNode* left,
    const struct RedBlackNode* right,
    RedBlackNode_CompareFuncType* compare_func) {
  assert(left != NULL);
  assert(right != NULL);

  return compare_func(left->ptr, right->ptr);
}

void RedBlackNode_Detach(struct RedBlackNode* node) {
  struct RedBlackNode* parent;

  parent = node->parent;
  if (parent == NULL) {
    return;
  }

  if (RedBlackNode_IsLeft(node)) {
    parent->left = NULL;
  } else {
    assert(RedBlackNode_IsRight(node));

    parent->right = NULL;
  }

  if (node->next != NULL) {
    node->next->previous = node->previous;
    node->next = NULL;
  }

  if (node->previous != NULL) {
    node->previous->next = node->next;
    node->previous = NULL;
  }

  node->parent = NULL;
}

struct RedBlackNode* RedBlackNode_GetFarNephew(
    const struct RedBlackNode* node) {
  struct RedBlackNode* sibling;

  sibling = RedBlackNode_GetSibling(node);
  if (sibling == NULL) {
    return NULL;
  }

  if (RedBlackNode_IsLeft(node)) {
    return sibling->right;
  } else {
    assert(RedBlackNode_IsRight(node));

    return sibling->left;
  }
}

struct RedBlackNode* RedBlackNode_GetGrandparent(
    const struct RedBlackNode* node) {
  assert(node != NULL);

  if (node->parent == NULL) {
    return NULL;
  }

  return node->parent->parent;
}

struct RedBlackNode* RedBlackNode_GetInOrderSuccessor(
    const struct RedBlackNode* node) {
  const struct RedBlackNode* successor;

  if (node->left != NULL) {
    for (successor = node->left;
        successor->right != NULL;
        successor = successor->right) {}
  } else if (node->right != NULL) {
    for (successor = node->right;
        successor->left != NULL;
        successor = successor->left) {}
  } else {
    assert(node->left == NULL);
    assert(node->right == NULL);

    return NULL;
  }

  return (struct RedBlackNode*)successor;
}

struct RedBlackNode* RedBlackNode_GetNearNephew(
    const struct RedBlackNode* node) {
  struct RedBlackNode* sibling;

  sibling = RedBlackNode_GetSibling(node);
  if (sibling == NULL) {
    return NULL;
  }

  if (RedBlackNode_IsLeft(node)) {
    return sibling->left;
  } else {
    assert(RedBlackNode_IsRight(node));

    return sibling->right;
  }
}

struct RedBlackNode* RedBlackNode_GetSibling(
    const struct RedBlackNode* node) {
  const struct RedBlackNode* parent;

  parent = node->parent;
  if (node->parent == NULL) {
    return NULL;
  }

  if (RedBlackNode_IsLeft(node)) {
    return (struct RedBlackNode*)parent->right;
  } else {
    assert(RedBlackNode_IsRight(node));

    return (struct RedBlackNode*)parent->left;
  }
}

struct RedBlackNode* RedBlackNode_GetUncle(const struct RedBlackNode* node) {
  struct RedBlackNode* grandparent;

  assert(node != NULL);

  grandparent = RedBlackNode_GetGrandparent(node);
  if (grandparent == NULL) {
    return NULL;
  }

  if (grandparent->left == node->parent) {
    return grandparent->right;
  } else {
    return grandparent->left;
  }
}

int RedBlackNode_IsBlack(const struct RedBlackNode* node) {
  return (node == NULL || node->color == RedBlackColor_kBlack);
}

int RedBlackNode_IsLeaf(const struct RedBlackNode* node) {
  return (node->left == NULL && node->right == NULL);
}

int RedBlackNode_IsLeft(const struct RedBlackNode* node) {
  const struct RedBlackNode* parent;

  parent = node->parent;
  if (parent == NULL) {
    return 0;
  }

  return (node == parent->left);
}

int RedBlackNode_IsRed(const struct RedBlackNode* node) {
  return (node != NULL && node->color == RedBlackColor_kRed);
}

int RedBlackNode_IsRight(const struct RedBlackNode* node) {
  const struct RedBlackNode* parent;

  parent = node->parent;
  if (parent == NULL) {
    return 0;
  }

  return (node == parent->right);
}

int RedBlackNode_IsRoot(const struct RedBlackNode* node) {
  return (node->parent == NULL);
}

void RedBlackNode_RotateAwayFromChild(
    struct RedBlackNode* node, struct RedBlackNode* child) {
  if (node->left == child) {
    RedBlackNode_RotateRight(node);
  } else if (node->right == child) {
    RedBlackNode_RotateLeft(node);
  }
}

void RedBlackNode_RotateAwayFromSibling(struct RedBlackNode* node) {
  if (RedBlackNode_IsRoot(node)) {
    return;
  }

  if (RedBlackNode_IsLeft(node)) {
    RedBlackNode_RotateLeft(node);
  } else {
    assert(RedBlackNode_IsRight(node));

    RedBlackNode_RotateRight(node);
  }
}

void RedBlackNode_RotateLeft(struct RedBlackNode* node) {
  struct RedBlackNode* old_parent;
  struct RedBlackNode* old_right;
  struct RedBlackNode* old_right_left;

  /* Set pointers related to right child. */
  old_right = node->right;
  assert(old_right != NULL);

  old_right_left = old_right->left;
  old_right->left = node;
  node->right = old_right_left;
  if (old_right_left != NULL) {
    old_right_left->parent = node;
  }

  /* If not root, then change the matching child pointer from parent. */
  old_parent = node->parent;
  if (old_parent != NULL) {
    if (RedBlackNode_IsLeft(node)) {
      old_parent->left = old_right;
    }
    else if (RedBlackNode_IsRight(node)) {
      old_parent->right = old_right;
    }
  }
  node->parent = old_right;
  old_right->parent = old_parent;
}

void RedBlackNode_RotateRight(struct RedBlackNode* node) {
  struct RedBlackNode* old_parent;
  struct RedBlackNode* old_left;
  struct RedBlackNode* old_left_right;

  /* Set pointers related to left child. */
  old_left = node->left;
  assert(old_left != NULL);

  old_left_right = old_left->right;
  old_left->right = node;
  node->left = old_left_right;
  if (old_left_right != NULL) {
    old_left_right->parent = node;
  }

  /* If not root, then change the matching child pointer from parent. */
  old_parent = node->parent;
  if (old_parent != NULL) {
    if (RedBlackNode_IsLeft(node)) {
      old_parent->left = old_left;
    }
    else if (RedBlackNode_IsRight(node)) {
      old_parent->right = old_left;
    }
  }
  node->parent = old_left;
  old_left->parent = old_parent;
}

void RedBlackNode_RotateTowardChild(
    struct RedBlackNode* node, struct RedBlackNode* child) {
  if (node->left == child) {
    RedBlackNode_RotateLeft(node);
  } else if (node->right == child) {
    RedBlackNode_RotateRight(node);
  }
}

void RedBlackNode_RotateTowardSibling(struct RedBlackNode* node) {
  if (RedBlackNode_IsRoot(node)) {
    return;
  }

  if (RedBlackNode_IsLeft(node)) {
    RedBlackNode_RotateRight(node);
  } else {
    assert(RedBlackNode_IsRight(node));

    RedBlackNode_RotateLeft(node);
  }
}

void RedBlackNode_SetLeft(
    struct RedBlackNode* node, struct RedBlackNode* child) {
  assert(node->left == NULL);

  node->left = child;
  child->parent = node;

  child->previous = node->previous;
  child->next = node;

  if (node->previous != NULL) {
    node->previous->next = child;
  }
  node->previous = child;
}

void RedBlackNode_SetRight(
    struct RedBlackNode* node, struct RedBlackNode* child) {
  assert(node->right == NULL);

  node->right = child;
  child->parent = node;

  child->next = node->next;
  child->previous = node;

  if (node->next != NULL) {
    node->next->previous = child;
  }
  node->next = child;
}

void RedBlackNode_SwapColor(
    struct RedBlackNode* left, struct RedBlackNode* right) {
  enum RedBlackColor temp;

  temp = left->color;
  left->color = right->color;
  right->color = temp;
}
