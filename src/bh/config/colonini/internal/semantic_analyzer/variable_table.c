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

#include <stddef.h>
#include <stdlib.h>

#include "bh/config/colonini/internal/semantic_analyzer/variable.h"

static int Variable_CompareNameAsVoid(const void* left, const void* right) {
  return Variable_CompareName(left, right);
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
    struct VariableTable* table, size_t initial_capacity) {
  table->vars = malloc(initial_capacity * sizeof(table->vars[0]));
  if (table->vars == NULL) {
    goto error;
  }

  table->unsorted_count = 0;
  table->sorted_count = 0;
  table->capacity = initial_capacity;

  return table;

error:
  return NULL;
}

void VariableTable_Deinit(struct VariableTable* table) {
  for ( ; table->unsorted_count > 0; --table->unsorted_count) {
    Variable_Deinit(&table->vars[table->unsorted_count - 1]);
  }
  table->sorted_count = 0;

  free(table->vars);
  table->capacity = 0;
  table->vars = NULL;
}

struct Variable* VariableTable_AddFromLine(
    struct VariableTable* table, const struct ParserLine* line) {
  struct Variable* init_variable_result;

  init_variable_result =
      Variable_Init(
          &table->vars[table->unsorted_count],
          &line->variant.assign_statement);
  if (init_variable_result == NULL) {
    goto error;
  }
  ++table->unsorted_count;

  return &table->vars[table->unsorted_count - 1];

error:
  return NULL;
}

struct Variable* VariableTable_SearchTableByName(
    struct VariableTable* table, const char* name, size_t length) {
  struct Variable search_key;
  struct Variable* search_result;

  /* Cast to char* is safe, no changes are going to be made to name. */
  search_key.name = (char*)name;
  search_key.name_length = length;

  search_result =
      bsearch(
          &search_key,
          table->vars,
          table->sorted_count,
          sizeof(table->vars[0]),
          &Variable_CompareNameAsVoid);

  /*
   * If result is found, return the result. If not found, but the table
   * is sorted, then the variable really isn't in the table.
   */
  if (search_result != NULL || table->sorted_count == table->unsorted_count) {
    return search_result;
  }

  qsort(
      table->vars,
      table->unsorted_count,
      sizeof(table->vars[0]),
      &Variable_CompareNameAsVoid);
  table->sorted_count = table->unsorted_count;

  return VariableTable_SearchTableByName(table, name, length);
}

int VariableTable_CheckLine(
    struct VariableTable* table, struct ParserLine* line) {
  struct Variable* search_result;
  struct KeyExpr* key;
  int resolve_result;

  /* Check for the variable in the table. */
  key = &line->variant.assign_statement.key_expr;
  search_result =
      VariableTable_SearchTableByName(
          table, key->constexpr.expr, key->constexpr.length);
  if (search_result == NULL) {
    struct Variable* add_result;

    add_result = VariableTable_AddFromLine(table, line);
    if (add_result == NULL) {
      goto error;
    }

    return 1;
  }

  if (Variable_EqualLineTypes(search_result, line)) {
    return 1;
  }

  /* Types don't match, so differences need to be upgraded to string type. */
  resolve_result = Variable_ResolveLineTypeDifference(search_result, line);
  return resolve_result;

error:
  return 0;
}
