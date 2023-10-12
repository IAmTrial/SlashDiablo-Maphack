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

#include "bh/common/string_util/ascii.h"

#include <stddef.h>
#include <wchar.h>

#include "bh/common/string_util/memstring.h"

/**
 * External
 */

/**
 * Define Ascii_ToLowerChar
 */

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_FUNC_NAME Ascii_ToLowerChar
#include "bh/common/string_util/internal/ascii/to_lower_char_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_FUNC_NAME Ascii_ToLowerWChar
#include "bh/common/string_util/internal/ascii/to_lower_char_template.h"

/**
 * Define Ascii_ToLowerStr
 */

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_TO_LOWER_CHAR_FUNC_NAME Ascii_ToLowerChar
#define T_FUNC_NAME Ascii_ToLowerStr
#include "bh/common/string_util/internal/ascii/to_lower_str_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_TO_LOWER_CHAR_FUNC_NAME Ascii_ToLowerWChar
#define T_FUNC_NAME Ascii_ToLowerWStr
#include "bh/common/string_util/internal/ascii/to_lower_str_template.h"

/**
 * Define Ascii_ToUpperChar
 */

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_FUNC_NAME Ascii_ToUpperChar
#include "bh/common/string_util/internal/ascii/to_upper_char_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_FUNC_NAME Ascii_ToUpperWChar
#include "bh/common/string_util/internal/ascii/to_upper_char_template.h"

/**
 * Define Ascii_ToUpperStr
 */

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_TO_UPPER_CHAR_FUNC_NAME Ascii_ToUpperChar
#define T_FUNC_NAME Ascii_ToUpperStr
#include "bh/common/string_util/internal/ascii/to_upper_str_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_TO_UPPER_CHAR_FUNC_NAME Ascii_ToUpperWChar
#define T_FUNC_NAME Ascii_ToUpperWStr
#include "bh/common/string_util/internal/ascii/to_upper_str_template.h"

/**
 * Define Ascii_TrimWhitespaceChars
 */

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_MEMCRSPN_FUNC_NAME MemCRSpn
#define T_MEMCPY_FUNC_NAME memcpy
#define T_MEMCSPN_FUNC_NAME MemCSpn
#define T_FUNC_NAME Ascii_TrimWhitespaceChars
#include "bh/common/string_util/internal/ascii/trim_whitespace_chars_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_MEMCRSPN_FUNC_NAME WMemCRSpn
#define T_MEMCPY_FUNC_NAME wmemcpy
#define T_MEMCSPN_FUNC_NAME WMemCSpn
#define T_FUNC_NAME Ascii_TrimWhitespaceWChars
#include "bh/common/string_util/internal/ascii/trim_whitespace_chars_template.h"
