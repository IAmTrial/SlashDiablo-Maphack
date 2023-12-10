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

#include "bh/config/colonini/internal/semantic_analyzer/typing_table.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "bh/common/data_struct/red_black_tree.h"
#include "bh/common/data_struct/red_black_tree/red_black_node.h"
#include "bh/config/colonini/internal/parser/assign_statement.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/const_expr_type.h"
#include "bh/config/colonini/internal/parser/parser_line.h"
#include "bh/config/colonini/internal/parser/parser_line_type.h"
#include "bh/config/colonini/internal/parser/value_expr_type.h"
#include "bh/config/colonini/internal/semantic_analyzer/typing.h"

static struct TypingTableEntry* TypingTableEntry_Init(
    struct TypingTableEntry* entry,
    const struct KeyExpr* keys,
    const struct ValueExpr* value) {
  struct Typing* typing_init_result;

  typing_init_result = Typing_Init(&entry->typing, keys, value);
  if (typing_init_result == NULL) {
    goto error;
  }

  return entry;

error:
  return NULL;
}

static void TypingTableEntry_Deinit(struct TypingTableEntry* entry) {
  entry->previous = NULL;
  Typing_Deinit(&entry->typing);
}

static int TypingTableEntry_CompareKeyName(
    const struct TypingTableEntry* left,
    const struct TypingTableEntry* right) {
  return Typing_CompareKeyName(&left->typing, &right->typing);
}

static int TypingTableEntry_CompareKeyNameAsVoid(
    const void* left, const void* right) {
  return TypingTableEntry_CompareKeyName(left, right);
}

static int TypingTable_InsertFromKeysAndValue(
    struct TypingTable* table,
    const struct KeyExpr* keys,
    const struct ValueExpr* value) {
  struct TypingTableEntry* entry_init_result;
  int tree_insert_succeeded;

  struct Typing* find_typing;
  struct TypingTableEntry* entry;

  find_typing = TypingTable_FindFromPrimaryKey(table, &keys->primary);
  if (find_typing != NULL) {
    return 0;
  }

  /* Typing is not found, so insert into the table. */
  entry = malloc(sizeof(*entry));
  if (entry == NULL) {
    goto error;
  }

  entry_init_result = TypingTableEntry_Init(entry, keys, value);
  if (entry_init_result == NULL) {
    goto error_free_entry;
  }

  tree_insert_succeeded =
      RedBlackTree_Insert(
          &table->tree, entry, &TypingTableEntry_CompareKeyNameAsVoid);
  if (!tree_insert_succeeded) {
    goto error_deinit_entry;
  }

  entry->previous = table->tail;
  table->tail = entry;
  ++table->count;

  return 1;

error_deinit_entry:
  TypingTableEntry_Deinit(entry);

error_free_entry:
  free(entry);

error:
  return 0;
}

static int TypingTableEntry_ResolveDiff(
    struct TypingTableEntry* left, struct TypingTableEntry* right) {
  return Typing_ResolveDiff(&left->typing, &right->typing);
}

/**
 * External
 */

struct TypingTable* TypingTable_Init(struct TypingTable* table) {
  struct RedBlackTree* tree_init_result;

  table->tail = NULL;
  table->count = 0;
  tree_init_result = RedBlackTree_Init(&table->tree);
  if (tree_init_result == NULL) {
    goto error;
  }

  return table;

error:
  return NULL;
}

void TypingTable_Deinit(struct TypingTable* table) {
  struct TypingTableEntry* current;
  struct TypingTableEntry* previous;

  RedBlackTree_Deinit(&table->tree);

  for (current = table->tail;
      current != NULL;
      current = previous, --table->count) {
    previous = current->previous;
    TypingTableEntry_Deinit(current);
  }
  table->tail = NULL;
}

struct Typing* TypingTable_FindFromPrimaryKey(
    const struct TypingTable* table, const struct ConstExpr* expr) {
  struct TypingTableEntry find_key;
  struct RedBlackNode* find_result;
  struct TypingTableEntry* find_entry;

  find_key.typing.key_name = expr;

  find_result =
      RedBlackTree_Find(
          &table->tree, &find_key, &TypingTableEntry_CompareKeyNameAsVoid);
  if (find_result == NULL) {
    return NULL;
  }

  find_entry = find_result->ptr;
  return &find_entry->typing;
}

int TypingTable_InsertOrResolveFromKeysAndValue(
    struct TypingTable* table,
    const struct KeyExpr* keys,
    const struct ValueExpr* value) {
  struct Typing* find_typing;

  find_typing = TypingTable_FindFromPrimaryKey(table, &keys->primary);
  if (find_typing != NULL) {
    return Typing_ResolveDiffFromKeysAndValue(find_typing, keys, value);
  }

  return TypingTable_InsertFromKeysAndValue(table, keys, value);
}

void TypingTable_Print(const struct TypingTable* table, FILE* file) {
  const struct TypingTableEntry* current;
  fprintf(file, "TypingTable:\n");
  for (current = table->tail; current != NULL; current = current->previous) {
    const struct Typing* typing;

    typing = &current->typing;
    fprintf(
        file,
        "- %s: Value type: [%s]; Subtype: %s\n",
        typing->key_name->expr,
        ValueExprType_GetDisplayName(typing->value_type, NULL),
        (typing->value_type == ValueExprType_kConst)
            ? ConstExprType_GetDisplayName(
                  typing->value_as_constexpr_type, NULL)
            : "N/A");
  }
}
