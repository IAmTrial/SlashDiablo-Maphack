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

/**
 * BH
 * Copyright (C) 2011  McGod
 *
 * All rights reserved.
 */

#ifndef BH_COMMON_STRING_UTIL_INTERNAL_INTEGER_FROM_DIGIT_CHAR_OF_BASE_H_
#define BH_COMMON_STRING_UTIL_INTERNAL_INTEGER_FROM_DIGIT_CHAR_OF_BASE_H_

#include <stddef.h>
#include <wchar.h>

#include "bh/common/preprocessor/template/identifier.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define T_Integer_FromDigitCharOfBase(T_CHAR) \
    TEMPLATE_IDENTIFIER_1(T_Integer_FromDigitCharOfBase, T_CHAR)

int* T_Integer_FromDigitCharOfBase(char)(int* value, char ch, int base);
int* T_Integer_FromDigitCharOfBase(wchar_t)(int* value, wchar_t ch, int base);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_COMMON_STRING_UTIL_INTERNAL_INTEGER_FROM_DIGIT_CHAR_OF_BASE_H_ */
