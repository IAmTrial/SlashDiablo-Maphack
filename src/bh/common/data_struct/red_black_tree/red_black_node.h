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

#ifndef BH_COMMON_DATA_STRUCT_RED_BLACK_TREE_RED_BLACK_NODE_H_
#define BH_COMMON_DATA_STRUCT_RED_BLACK_TREE_RED_BLACK_NODE_H_

#include "bh/common/data_struct/red_black_tree/red_black_color.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * Red-black tree node that points to the current data and allows
 * traversal to the previous and next elements in the tree.
 *
 * Does not manage the pointer to the data that is pointed to.
 */
struct RedBlackNode {
  enum RedBlackColor color;
  struct RedBlackNode* previous;
  struct RedBlackNode* next;

  struct RedBlackNode* parent;
  struct RedBlackNode* left;
  struct RedBlackNode* right;

  /** Unmanaged pointer to the current data. */
  void* ptr;
};

typedef int RedBlackNode_CompareFuncType(
    const void* left, const void* right);

struct RedBlackNode* RedBlackNode_InitDefault(struct RedBlackNode* node);

void RedBlackNode_Deinit(struct RedBlackNode* node);

void RedBlackNode_AddBlack(struct RedBlackNode* node);

int RedBlackNode_ComparePointerData(
    const struct RedBlackNode* left,
    const struct RedBlackNode* right,
    RedBlackNode_CompareFuncType* compare_func);

void RedBlackNode_Detach(struct RedBlackNode* node);

struct RedBlackNode* RedBlackNode_GetFarNephew(
    const struct RedBlackNode* node);

struct RedBlackNode* RedBlackNode_GetGrandparent(
    const struct RedBlackNode* node);

struct RedBlackNode* RedBlackNode_GetInOrderSuccessor(
    const struct RedBlackNode* node);

struct RedBlackNode* RedBlackNode_GetNearNephew(
    const struct RedBlackNode* node);

struct RedBlackNode* RedBlackNode_GetSibling(const struct RedBlackNode* node);

struct RedBlackNode* RedBlackNode_GetUncle(const struct RedBlackNode* node);

int RedBlackNode_IsBlack(const struct RedBlackNode* node);

int RedBlackNode_IsLeaf(const struct RedBlackNode* node);

int RedBlackNode_IsLeft(const struct RedBlackNode* node);

int RedBlackNode_IsRed(const struct RedBlackNode* node);

int RedBlackNode_IsRight(const struct RedBlackNode* node);

int RedBlackNode_IsRoot(const struct RedBlackNode* node);

void RedBlackNode_RotateAwayFromChild(
    struct RedBlackNode* node, struct RedBlackNode* child);

void RedBlackNode_RotateAwayFromSibling(struct RedBlackNode* node);

void RedBlackNode_RotateLeft(struct RedBlackNode* node);

void RedBlackNode_RotateRight(struct RedBlackNode* node);

void RedBlackNode_RotateTowardChild(
    struct RedBlackNode* node, struct RedBlackNode* child);

void RedBlackNode_RotateTowardSibling(struct RedBlackNode* node);

void RedBlackNode_SetLeft(
    struct RedBlackNode* node, struct RedBlackNode* child);

void RedBlackNode_SetRight(
    struct RedBlackNode* node, struct RedBlackNode* child);

void RedBlackNode_SwapColor(
    struct RedBlackNode* left, struct RedBlackNode* right);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_DATA_STRUCT_RED_BLACK_TREE_RED_BLACK_NODE_H_ */
