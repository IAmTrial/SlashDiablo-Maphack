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

#ifndef BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_H_
#define BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_H_

#include <stddef.h>

#include "bh/config/colonini/internal/parser.h"
#include "bh/config/colonini/internal/semantic_analyzer/typing_table.h"
#include "bh/config/colonini/internal/semantic_analyzer/variable_table.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct SemanticAnalyzer {
  struct TypingTable typing_table;
  struct VariableTable var_table;
};

struct SemanticAnalyzer* SemanticAnalyzer_Init(
    struct SemanticAnalyzer* analyzer,
    const struct Parser* parser);

void SemanticAnalyzer_Deinit(struct SemanticAnalyzer* analyzer);

/**
 * Adds the semantic context from a contiguous array of ParserLine.
 * Returns a non-zero value on success, or else zero on failure.
 */
int SemanticAnalyzer_LoadLines(
    struct SemanticAnalyzer* analyzer,
    const struct Parser* parser);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_SEMANTIC_ANALYZER_H_ */
