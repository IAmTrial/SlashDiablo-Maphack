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

#include "bh/common/string_util/integer.h"

#include <limits.h>
#include <stddef.h>
#include <wchar.h>

#include "bh/common/string_util/ascii.h"

/**
 * External
 */

/**
 * Define Integer_FromDigitChar
 */

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_TO_LOWER_CHAR_FUNC_NAME Ascii_ToLowerChar
#define T_FUNC_NAME Integer_FromDigitChar
#include "bh/common/string_util/internal/integer/from_digit_char_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_TO_LOWER_CHAR_FUNC_NAME Ascii_ToLowerWChar
#define T_FUNC_NAME Integer_FromDigitWChar
#include "bh/common/string_util/internal/integer/from_digit_char_template.h"

/**
 * Integer_FromStrToInt
 */

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_INT int
#define T_INT_MIN INT_MIN
#define T_INT_MAX INT_MAX
#define T_FROM_DIGIT_CHAR_FUNC_NAME Integer_FromDigitChar
#define T_GET_BASE_FROM_PREFIX_STR_FUNC_NAME Integer_GetBaseFromPrefixStr
#define T_FUNC_NAME Integer_FromStrToInt
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_INT int
#define T_INT_MIN INT_MIN
#define T_INT_MAX INT_MAX
#define T_FROM_DIGIT_CHAR_FUNC_NAME Integer_FromDigitWChar
#define T_GET_BASE_FROM_PREFIX_STR_FUNC_NAME Integer_GetBaseFromPrefixWStr
#define T_FUNC_NAME Integer_FromWStrToInt
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_INT unsigned int
#define T_INT_MIN 0u
#define T_INT_MAX UINT_MAX
#define T_FROM_DIGIT_CHAR_FUNC_NAME Integer_FromDigitChar
#define T_GET_BASE_FROM_PREFIX_STR_FUNC_NAME Integer_GetBaseFromPrefixStr
#define T_FUNC_NAME Integer_FromStrToUInt
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_INT unsigned int
#define T_INT_MIN 0u
#define T_INT_MAX UINT_MAX
#define T_FROM_DIGIT_CHAR_FUNC_NAME Integer_FromDigitWChar
#define T_GET_BASE_FROM_PREFIX_STR_FUNC_NAME Integer_GetBaseFromPrefixWStr
#define T_FUNC_NAME Integer_FromWStrToUInt
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

/**
 * Define Integer_GetBaseFromPrefixStr
 */

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_FUNC_NAME Integer_GetBaseFromPrefixStr
#include "bh/common/string_util/internal/integer/get_base_from_prefix_str_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_FUNC_NAME Integer_GetBaseFromPrefixWStr
#include "bh/common/string_util/internal/integer/get_base_from_prefix_str_template.h"

/**
 * Integer_IsDigitCharOfBase
 */

#define T_CHAR char
#define T_FROM_DIGIT_CHAR_FUNC_NAME Integer_FromDigitChar
#define T_FUNC_NAME Integer_IsDigitCharOfBase
#include "bh/common/string_util/internal/integer/is_digit_char_of_base_template.h"

#define T_CHAR wchar_t
#define T_FROM_DIGIT_CHAR_FUNC_NAME Integer_FromDigitWChar
#define T_FUNC_NAME Integer_IsDigitWCharOfBase
#include "bh/common/string_util/internal/integer/is_digit_char_of_base_template.h"
