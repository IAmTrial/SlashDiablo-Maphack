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

#ifndef BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_VARIABLE_TABLE_H_
#define BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_VARIABLE_TABLE_H_

#include <stddef.h>

#include "bh/config/colonini/internal/parser.h"
#include "bh/config/colonini/internal/semantic_analyzer/variable.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct VariableTable {
  struct Variable* vars;
  size_t capacity;
  size_t unsorted_count;
  size_t sorted_count;
};

struct VariableTable* VariableTable_Init(
    struct VariableTable* table, size_t initial_capacity);

void VariableTable_Deinit(struct VariableTable* table);

struct Variable* VariableTable_AddFromLine(
    struct VariableTable* table, const struct ParserLine* line);

int VariableTable_CheckLine(
    struct VariableTable* table, struct ParserLine* line);

/**
 * Returns whether a duplicate variable (matching by name and subkeys)
 * is added to the table.
 *
 * Precondition: Table must be sorted via VariableTable_Sort.
 */
int VariableTable_ContainsDuplicate(const struct VariableTable* table);

/**
 * Resolves the type differences of keys, subkeys, and value by
 * converting to string, if possible.
 */
int VariableTable_ResolveTypeDiffs(struct VariableTable* table);

struct Variable* VariableTable_SearchTableByName(
    struct VariableTable* table, const char* name, size_t length);

void VariableTable_Sort(struct VariableTable* table);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_VARIABLE_TABLE_H_ */
