/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2022  SlashDiablo Team
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

#ifndef TESTS_BH_CONFIG_COLONINI_INTERNAL_TEST_HELPER_H_
#define TESTS_BH_CONFIG_COLONINI_INTERNAL_TEST_HELPER_H_

#include <stdarg.h>
#include <stddef.h>

#include "bh/config/colonini/internal/lexer.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* va_args: (const char* str, int is_token, ...) */
void LexerLineSetUp(
    struct LexerLine* line, struct LexerString* strs, size_t count, ...);

void LexerLineVSetUp(
    struct LexerLine* line,
    struct LexerString* strs,
    size_t count,
    va_list args);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* TESTS_BH_CONFIG_COLONINI_INTERNAL_TEST_HELPER_H_ */
