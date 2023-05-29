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

#ifndef BH_COMMON_DATA_STRUCT_RED_BLACK_TREE_H_
#define BH_COMMON_DATA_STRUCT_RED_BLACK_TREE_H_

#include <stddef.h>

#include "bh/common/data_struct/red_black_tree/red_black_node.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * Red-black tree node that performs insertion with O(log(n)) time
 * complexity. Allows in-order traversal of elements in the node.
 *
 * Only manages space allocated for nodes, and not the data that the
 * nodes point to.
 */
struct RedBlackTree {
  size_t count;
  struct RedBlackNode* first;
  struct RedBlackNode* last;

  struct RedBlackNode* root;
};

/**
 * Initializes an instance of a RedBlackTree.
 */
struct RedBlackTree* RedBlackTree_Init(struct RedBlackTree* tree);

/**
 * Deinitializes a RedBlackTree, freeing allocated resources.
 */
void RedBlackTree_Deinit(struct RedBlackTree* tree);

/**
 * Returns whether the tree contains the specified data.
 */
int RedBlackTree_Contains(
    struct RedBlackTree* tree,
    const void* data,
    RedBlackNode_CompareFuncType* compare_func);

/**
 * Returns the comparison of the data of two trees, using in-order
 * traversal of the tree.
 */
int RedBlackTree_Compare(
    const struct RedBlackTree* left,
    const struct RedBlackTree* right,
    RedBlackNode_CompareFuncType* compare_func);

/**
 * Inserts a pointer to data into the tree, but only if the data does
 * not already exist in the tree. Returns whether the data was inserted
 * into the tree.
 */
int RedBlackTree_Insert(
    struct RedBlackTree* tree,
    void* data,
    RedBlackNode_CompareFuncType* compare_func);

/**
 * Returns a pointer to the node that is equal to the specified data.
 */
struct RedBlackNode* RedBlackTree_Find(
    const struct RedBlackTree* tree,
    const void* data,
    RedBlackNode_CompareFuncType* compare_func);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_DATA_STRUCT_RED_BLACK_TREE_H_ */
