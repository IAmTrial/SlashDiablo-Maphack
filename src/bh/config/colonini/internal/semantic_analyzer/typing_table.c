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
#include <stdlib.h>

#include "bh/common/data_struct/red_black_tree.h"
#include "bh/common/data_struct/red_black_tree/red_black_node.h"
#include "bh/config/colonini/internal/parser/assign_statement.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/parser_line.h"
#include "bh/config/colonini/internal/parser/parser_line_type.h"
#include "bh/config/colonini/internal/semantic_analyzer/typing.h"

static int Typing_CompareKeyNameAsVoid(const void* left, const void* right) {
  return Typing_CompareKeyName(left, right);
}

static struct Typing* TypingTable_SearchWithPrimaryKeyFromAssignStatement(
    const struct TypingTable* table,
    const struct AssignStatement* assign_statement) {
  return TypingTable_SearchWithPrimaryKey(
      table, &assign_statement->key_expr.constexpr);
}

static struct Typing* TypingTable_SearchWithPrimaryKeyFromLine(
    const struct TypingTable* table, const struct ParserLine* line) {
  switch (line->type) {
    case ParserLineType_kAssignStatement: {
      return TypingTable_SearchWithPrimaryKeyFromAssignStatement(
          table, &line->variant.assign_statement);
    }

    default: {
      assert(0 && "This should never happen.");
      return NULL;
    }
  }
}

/**
 * External
 */

struct TypingTable* TypingTable_Init(
    struct TypingTable* table, size_t capacity) {
  struct RedBlackTree* init_tree_result;

  table->typings = malloc(capacity * sizeof(table->typings[0]));
  if (table->typings == NULL) {
    goto error;
  }
  table->capacity = capacity;
  table->count = 0;

  init_tree_result = RedBlackTree_Init(&table->tree);
  if (init_tree_result == NULL) {
    goto error_free_typings;
  }

  return table;

error_free_typings:
  table->capacity = 0;
  free(table->typings);
  table->typings = NULL;

error:
  return NULL;
}

void TypingTable_Deinit(struct TypingTable* table) {
  RedBlackTree_Deinit(&table->tree);
  while (table->count-- > 0) {
    Typing_Deinit(&table->typings[table->count]);
  }
  table->capacity = 0;
  free(table->typings);
  table->typings = NULL;
}

struct Typing* TypingTable_InsertFromLine(
    struct TypingTable* table, const struct ParserLine* line) {
  struct Typing* init_typing_result;

  int insert_succeeded;

  if (table->count == table->capacity) {
    return 0;
  }

  init_typing_result = Typing_Init(&table->typings[table->count], line);
  if (init_typing_result == NULL) {
    goto error;
  }

  insert_succeeded =
      RedBlackTree_Insert(
          &table->tree,
          &table->typings[table->count],
          &Typing_CompareKeyNameAsVoid);
  if (!insert_succeeded) {
    Typing_Deinit(&table->typings[table->count]);
    return NULL;
  }

  ++table->count;

  return &table->typings[table->count - 1];

error:
  return NULL;
}

struct Typing* TypingTable_InsertOrResolveFromLine(
    struct TypingTable* table, const struct ParserLine* line) {
  struct Typing* search_result;
  int resolve_succeeded;

  search_result = TypingTable_SearchWithPrimaryKeyFromLine(table, line);
  if (search_result == NULL) {
    return TypingTable_InsertFromLine(table, line);
  }

  resolve_succeeded = Typing_ResolveLineDiff(search_result, line);
  if (!resolve_succeeded) {
    return NULL;
  }

  return search_result;
}

struct Typing* TypingTable_SearchWithPrimaryKey(
    const struct TypingTable* table, const struct ConstExpr* expr) {
  struct Typing search_key;
  struct RedBlackNode* search_result;

  search_key.key_name = expr;

  search_result =
      RedBlackTree_Find(
          &table->tree, &search_key, &Typing_CompareKeyNameAsVoid);
  if (search_result == NULL) {
    return NULL;
  }

  return search_result->ptr;
}
