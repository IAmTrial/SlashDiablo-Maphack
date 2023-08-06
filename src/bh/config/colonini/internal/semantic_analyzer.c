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

#include "bh/config/colonini/internal/semantic_analyzer.h"

#include <assert.h>
#include <stddef.h>

#include "bh/config/colonini/internal/parser.h"
#include "bh/config/colonini/internal/parser/parser_line_type.h"
#include "bh/config/colonini/internal/semantic_analyzer/variable.h"
#include "bh/config/colonini/internal/semantic_analyzer/variable_table.h"

/**
 * External
 */

struct SemanticAnalyzer* SemanticAnalyzer_Init(
    struct SemanticAnalyzer* analyzer,
    const struct ParserLine* lines,
    size_t count) {
  struct TypingTable* typing_table_init_result;
  struct VariableTable* var_table_init_result;

  size_t i;
  size_t var_count;

  var_count = 0;
  for (i = 0; i < count; ++i) {
    switch (lines[i].type) {
      case ParserLineType_kAssignStatement: {
        ++var_count;
        break;
      }

      case ParserLineType_kNoOp: {
        break;
      }

      default: {
        assert(0 && "This should never happen.");
        return NULL;
      }
    }
  }

  typing_table_init_result =
      TypingTable_Init(&analyzer->typing_table, var_count);
  if (typing_table_init_result == NULL) {
    goto error;
  }

  var_table_init_result = VariableTable_Init(&analyzer->var_table, var_count);
  if (var_table_init_result == NULL) {
    goto error_deinit_typing_table;
  }

  return analyzer;

error_deinit_typing_table:
  TypingTable_Deinit(&analyzer->typing_table);

error:
  return NULL;
}

void SemanticAnalyzer_Deinit(struct SemanticAnalyzer* analyzer) {
  VariableTable_Deinit(&analyzer->var_table);
}

int SemanticAnalyzer_LoadLines(
    struct SemanticAnalyzer* analyzer,
    const struct ParserLine* lines,
    size_t count) {
  size_t i;

  /* Determine typing for each variable. */
  for (i = 0; i < count; ++i) {
    struct Typing* insert_result;

    if (lines[i].type == ParserLineType_kNoOp) {
      continue;
    }

    insert_result =
        TypingTable_InsertOrResolveFromLine(&analyzer->typing_table, &lines[i]);
    if (insert_result == NULL) {
      goto error;
    }
  }

  /* Add to the variable table. */
  for (i = 0; i < count; ++i) {
    struct Variable* add_result;

    const struct Typing* typing;

    if (lines[i].type == ParserLineType_kNoOp) {
      continue;
    }

    add_result =
        VariableTable_InsertFromLine(&analyzer->var_table, &lines[i]);
    if (add_result == NULL) {
      return 0;
    }
  }

  return 1;

error:
  return 0;
}
