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

#include <stddef.h>

#include "bh/config/colonini/internal/parser.h"
#include "bh/config/colonini/internal/semantic_analyzer/variable.h"
#include "bh/config/colonini/internal/semantic_analyzer/variable_table.h"

/**
 * External
 */

int SemanticAnalyzer_Check(struct ParserLine* lines, size_t count) {
  size_t i;

  int check_result;
  struct VariableTable var_table;
  struct VariableTable* init_var_table_result;

  init_var_table_result = VariableTable_Init(&var_table, count);
  if (init_var_table_result == NULL) {
    goto error;
  }

  /*
   * First run is to load all of the variables. Second run is to
   * correct the variable types.
   */
  for (i = 0; i < 2; ++i) {
    size_t i_lines;
    for (i_lines = 0; i_lines < count; ++i_lines) {
      check_result = VariableTable_CheckLine(&var_table, &lines[i_lines]);
      if (!check_result) {
        break;
      }
    }
  }

  VariableTable_Deinit(&var_table);
  return check_result;

error:
  return 0;
}
