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

#include "bh/config/colonini/internal/semantic_analyzer/key_table.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "bh/common/data_struct/red_black_tree.h"
#include "bh/common/data_struct/red_black_tree/red_black_node.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/key_expr.h"

static struct KeyTableEntry* KeyTableEntry_Init(
    struct KeyTableEntry* entry, const struct ConstExpr* expr) {
  struct KeyTable* table_init_result;

  entry->expr = expr;
  table_init_result = KeyTable_Init(&entry->table);
  if (table_init_result == NULL) {
    goto error;
  }
  entry->previous = NULL;

  return entry;

error:
  return NULL;
}

static void KeyTableEntry_Deinit(struct KeyTableEntry* entry) {
  entry->previous = NULL;
  KeyTable_Deinit(&entry->table);
  entry->expr = NULL;
}

static int KeyTableEntry_CompareExpr(
    const struct KeyTableEntry* left, const struct KeyTableEntry* right) {
  return ConstExpr_CompareExprAsString(left->expr, right->expr);
}

static int KeyTableEntry_CompareExprAsVoid(
    const void* left, const void* right) {
  return KeyTableEntry_CompareExpr(left, right);
}

static struct ConstExpr* KeyExpr_GetKeyAtLevel(
    const struct KeyExpr* keys, size_t key_level) {
  return (struct ConstExpr*)((key_level == 0)
          ? &keys->primary
          : &keys->subscripts[key_level - 1].expr);
}

static struct KeyTable* KeyTable_FindFromKeysAtLevel(
    const struct KeyTable* table,
    const struct KeyExpr* keys,
    size_t key_level) {
  const struct ConstExpr* find_key;
  struct KeyTable* found_table;

  assert(key_level <= keys->subscript_count);
  find_key = KeyExpr_GetKeyAtLevel(keys, key_level);
  found_table = KeyTable_FindFromKey(table, find_key);

  return (key_level == keys->subscript_count)
      ? found_table
      : KeyTable_FindFromKeysAtLevel(table, keys, key_level + 1);
}

/**
 * Inserts the key and subkeys of a key expression into the key table
 * at the specified key level. Key level zero is the primary key, and
 * key levels above zero are the subkeys. Returns the total count of
 * key and subkeys inserted.
 */
static size_t KeyTable_InsertFromKeysAtLevel(
    struct KeyTable* table, const struct KeyExpr* keys, size_t key_level) {
  size_t current_inserted;
  const struct ConstExpr* find_key;
  struct KeyTable* subtable;
  size_t subkeys_insert_count;

  assert(key_level <= keys->subscript_count);
  find_key = KeyExpr_GetKeyAtLevel(keys, key_level);

  current_inserted = KeyTable_InsertFromKey(table, find_key);
  subtable = KeyTable_FindFromKey(table, find_key);

  subkeys_insert_count =
      (key_level == keys->subscript_count)
          ? 0
          : KeyTable_InsertFromKeysAtLevel(subtable, keys, key_level + 1);

  return subkeys_insert_count + current_inserted;

error:
  return 0;
}

static void KeyTable_PrintInner(
    const struct KeyTable* table, FILE* file, size_t indent) {
  size_t i;
  const struct KeyTableEntry* current;

  for (current = table->tail; current != NULL; current = current->previous) {
    for (i = 0; i < indent; ++i) {
      fprintf(file, "-");
    }
    if (indent != 0) {
      fprintf(file, " ");
    }
    fprintf(file, "%s\n", current->expr->expr);
    KeyTable_PrintInner(&current->table, file, indent + 1);
  }
}

/**
 * External
 */

struct KeyTable* KeyTable_Init(struct KeyTable* table) {
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

void KeyTable_Deinit(struct KeyTable* table) {
  struct KeyTableEntry* current;
  struct KeyTableEntry* previous;

  RedBlackTree_Deinit(&table->tree);

  for (current = table->tail;
      current != NULL;
      current = previous, --table->count) {
    previous = current->previous;
    KeyTableEntry_Deinit(current);
  }
  table->tail = NULL;
}

struct KeyTable* KeyTable_FindFromKey(
    const struct KeyTable* table, const struct ConstExpr* key) {
  struct KeyTableEntry find_key;
  struct RedBlackNode* find_node;
  struct KeyTableEntry* find_entry;

  find_key.expr = key;

  find_node =
      RedBlackTree_Find(
          &table->tree, &find_key, &KeyTableEntry_CompareExprAsVoid);
  if (find_node == NULL) {
    return NULL;
  }

  find_entry = find_node->ptr;
  return &find_entry->table;
}

struct KeyTable* KeyTable_FindFromKeys(
    const struct KeyTable* table, const struct KeyExpr* keys) {
  return KeyTable_FindFromKeysAtLevel(table, keys, 0);
}

size_t KeyTable_InsertFromKey(
    struct KeyTable* table, const struct ConstExpr* key) {
  struct KeyTableEntry* entry_init_result;
  int tree_insert_succeeded;

  struct KeyTable* subtable;
  struct KeyTableEntry* entry;
  size_t subkeys_insert_count;

  subtable = KeyTable_FindFromKey(table, key);
  if (subtable != NULL) {
    return 0;
  }

  /* Node not found, so it needs to be added. */
  entry = malloc(sizeof(*entry));
  if (entry == NULL) {
    goto error;
  }

  entry_init_result = KeyTableEntry_Init(entry, key);
  if (entry_init_result == NULL) {
    goto error_free_entry;
  }

  tree_insert_succeeded =
      RedBlackTree_Insert(
          &table->tree, entry, &KeyTableEntry_CompareExprAsVoid);
  if (!tree_insert_succeeded) {
    goto error_deinit_entry;
  }

  entry->previous = table->tail;
  table->tail = entry;
  ++table->count;

  return 1;

error_deinit_entry:
  KeyTableEntry_Deinit(entry);

error_free_entry:
  free(entry);

error:
  return 0;
}

size_t KeyTable_InsertFromKeys(
    struct KeyTable* table, const struct KeyExpr* keys) {
  return KeyTable_InsertFromKeysAtLevel(table, keys, 0);
}

void KeyTable_Print(const struct KeyTable* table, FILE* file) {
  fprintf(file, "KeyTable:\n");
  KeyTable_PrintInner(table, file, 0);
}
