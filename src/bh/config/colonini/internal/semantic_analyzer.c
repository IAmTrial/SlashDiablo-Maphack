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
#include "bh/config/colonini/internal/semantic_analyzer/variable.h"
#include "bh/config/colonini/internal/semantic_analyzer/variable_table.h"

/**
 * External
 */

struct SemanticAnalyzer* SemanticAnalyzer_Init(
    struct SemanticAnalyzer* analyzer,
    const struct ParserLine* lines,
    size_t count) {
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

  var_table_init_result = VariableTable_Init(&analyzer->var_table, var_count);
  if (var_table_init_result == NULL) {
    goto error;
  }

  return analyzer;

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

  for (i = 0; i < count; ++i) {
    switch (lines[i].type) {
      case ParserLineType_kAssignStatement: {
        struct Variable* add_result;

        add_result =
            VariableTable_AddFromLine(&analyzer->var_table, &lines[i]);
        if (add_result == NULL) {
          goto error;
        }
        break;
      }

      case ParserLineType_kNoOp: {
        break;
      }

      default: {
        assert(0 && "This should never happen.");
        return 0;
      }
    }
  }

  VariableTable_Sort(&analyzer->var_table);

  return 1;

error:
  return 0;
}

int SemanticAnalyzer_CheckLines(
    struct SemanticAnalyzer* analyzer,
    struct ParserLine* lines,
    size_t count) {
  size_t i;

  int resolve_result;
  int check_result;

  if (VariableTable_ContainsDuplicate(&analyzer->var_table)) {
    return 0;
  }

  resolve_result = VariableTable_ResolveTypeDiffs(&analyzer->var_table);
  if (!resolve_result) {
    return 0;
  }

  for (i = 0; i < count; ++i) {
    check_result =
        VariableTable_CheckLine(&analyzer->var_table, &lines[i]);
    if (!check_result) {
      break;
    }
  }

  return check_result;

error:
  return 0;
}
