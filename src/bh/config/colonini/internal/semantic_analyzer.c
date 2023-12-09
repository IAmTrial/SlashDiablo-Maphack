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
#include <stdio.h>

#include "bh/config/colonini/internal/parser.h"
#include "bh/config/colonini/internal/parser/parser_line_type.h"
#include "bh/config/colonini/internal/semantic_analyzer/key_table.h"
#include "bh/config/colonini/internal/semantic_analyzer/typing_table.h"

static int SemanticAnalyzer_LoadLine(
    struct SemanticAnalyzer* analyzer,
    const struct ParserLine* line) {
  switch (line->type) {
    case ParserLineType_kNoOp: {
      return 1;
    }

    case ParserLineType_kAssignStatement: {
      const struct AssignStatement* assign_statement;
      int typing_insert_or_resolve_succeeded;
      size_t key_table_insert_count;

      assign_statement = &line->variant.assign_statement;

      /* Determine typing for each variable. */
      typing_insert_or_resolve_succeeded =
          TypingTable_InsertOrResolveFromKeysAndValue(
              &analyzer->typing_table,
              &assign_statement->key_expr,
              &assign_statement->value_expr);
      if (!typing_insert_or_resolve_succeeded) {
        goto error;
      }

      /* Add to the key table. */
      key_table_insert_count =
          KeyTable_InsertFromKeys(
              &analyzer->key_table, &assign_statement->key_expr);
      if (key_table_insert_count == 0) {
        return 0;
      }

      return 1;
    }

    case ParserLineType_kUnspecified: {
      assert(0 && ParserLineType_kUnspecified && "This should never happen.");
      return 0;
    }

    case ParserLineType_kInvalid: {
      assert(0 && ParserLineType_kInvalid && "This should never happen.");
      return 0;
    }

    default: {
      assert(0 && "This should never happen.");
      return 0;
    }
  }

error:
  return 0;
}

/**
 * External
 */

struct SemanticAnalyzer* SemanticAnalyzer_Init(
    struct SemanticAnalyzer* analyzer,
    const struct Parser* parser) {
  struct TypingTable* typing_table_init_result;
  struct KeyTable* key_table_init_result;

  typing_table_init_result =
      TypingTable_Init(&analyzer->typing_table);
  if (typing_table_init_result == NULL) {
    goto error;
  }

  key_table_init_result = KeyTable_Init(&analyzer->key_table);
  if (key_table_init_result == NULL) {
    goto error_deinit_typing_table;
  }

  return analyzer;

error_deinit_typing_table:
  TypingTable_Deinit(&analyzer->typing_table);

error:
  return NULL;
}

void SemanticAnalyzer_Deinit(struct SemanticAnalyzer* analyzer) {
  KeyTable_Deinit(&analyzer->key_table);
  TypingTable_Deinit(&analyzer->typing_table);
}

int SemanticAnalyzer_LoadLines(
    struct SemanticAnalyzer* analyzer,
    const struct Parser* parser) {
  size_t i;

  for (i = 0; i < parser->line_count; ++i) {
    int load_line_succeeded;

    load_line_succeeded =
        SemanticAnalyzer_LoadLine(analyzer, &parser->lines[i]);
    if (!load_line_succeeded) {
      goto error;
    }
  }

  return 1;

error:
  return 0;
}

void SemanticAnalyzer_Print(
    const struct SemanticAnalyzer* analyzer, FILE* file) {
  fprintf(file, "Semantic Analyzer:\n\n");
  KeyTable_Print(&analyzer->key_table, file);
  fprintf(file, "\n");
  TypingTable_Print(&analyzer->typing_table, file);
}
