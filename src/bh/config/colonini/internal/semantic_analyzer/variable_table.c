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

#include "bh/config/colonini/internal/semantic_analyzer/variable_table.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "bh/common/data_struct/red_black_tree.h"
#include "bh/config/colonini/internal/parser/parser_line.h"
#include "bh/config/colonini/internal/semantic_analyzer/variable.h"

static int Variable_CompareKeysAsStringsAsVoid(
    const void* left, const void* right) {
  return Variable_CompareKeysAsStrings(left, right);
}

static struct VariableTable* VariableTable_Reserve(
    struct VariableTable* table, size_t new_capacity) {
  void* alloc_result;
  if (table->capacity == new_capacity) {
    return table;
  }

  alloc_result = realloc(table->vars, new_capacity * sizeof(table->vars[0]));
  if (alloc_result == NULL) {
    goto error;
  }
  table->vars = alloc_result;
  table->capacity = new_capacity;

  return table;

error:
  return NULL;
}

/**
 * External
 */

struct VariableTable* VariableTable_Init(
    struct VariableTable* table, size_t capacity) {
  struct RedBlackTree* tree_init_result;

  table->vars = malloc(capacity * sizeof(table->vars[0]));
  if (table->vars == NULL) {
    goto error;
  }
  table->count = 0;
  table->capacity = capacity;

  tree_init_result = RedBlackTree_Init(&table->tree);
  if (tree_init_result == NULL) {
    goto error_free_vars;
  }

  return table;

error_free_vars:
  free(table->vars);
  table->vars = NULL;

error:
  return NULL;
}

void VariableTable_Deinit(struct VariableTable* table) {
  RedBlackTree_Deinit(&table->tree);

  while (table->count-- > 0) {
    Variable_Deinit(&table->vars[table->count]);
  }

  table->capacity = 0;
  free(table->vars);
  table->vars = NULL;
}

struct Variable* VariableTable_InsertFromLine(
    struct VariableTable* table, const struct ParserLine* line) {
  size_t i;

  struct Variable* init_result;
  int insert_succeeded;

  init_result = Variable_Init(&table->vars[table->count], line);
  if (init_result == NULL) {
    goto error;
  }

  insert_succeeded =
      RedBlackTree_Insert(
          &table->tree,
          &table->vars[table->count],
          &Variable_CompareKeysAsStringsAsVoid);
  if (!insert_succeeded) {
    Variable_Deinit(&table->vars[table->count]);
    return NULL;
  }

  ++table->count;

  return &table->vars[table->count - 1];

error:
  return NULL;
}
