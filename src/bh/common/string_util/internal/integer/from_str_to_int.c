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

#include "bh/common/string_util/internal/integer/from_str_to_int.h"

#include <limits.h>
#include <stddef.h>
#include <wchar.h>

#include "bh/common/preprocessor/template/typedef.h"

/**
 * External
 */

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_INT signed_char
#define T_INT_MIN SCHAR_MIN
#define T_INT_MAX SCHAR_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_INT signed_char
#define T_INT_MIN SCHAR_MIN
#define T_INT_MAX SCHAR_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_INT short
#define T_INT_MIN SHRT_MIN
#define T_INT_MAX SHRT_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_INT short
#define T_INT_MIN SHRT_MIN
#define T_INT_MAX SHRT_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_INT int
#define T_INT_MIN INT_MIN
#define T_INT_MAX INT_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_INT int
#define T_INT_MIN INT_MIN
#define T_INT_MAX INT_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_INT long
#define T_INT_MIN LONG_MIN
#define T_INT_MAX LONG_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_INT long
#define T_INT_MIN LONG_MIN
#define T_INT_MAX LONG_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_INT unsigned_char
#define T_INT_MIN 0u
#define T_INT_MAX UCHAR_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_INT unsigned_char
#define T_INT_MIN 0u
#define T_INT_MAX UCHAR_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_INT unsigned_short
#define T_INT_MIN 0u
#define T_INT_MAX USHRT_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_INT unsigned_short
#define T_INT_MIN 0u
#define T_INT_MAX USHRT_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_INT unsigned_int
#define T_INT_MIN 0u
#define T_INT_MAX UINT_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_INT unsigned_int
#define T_INT_MIN 0u
#define T_INT_MAX UINT_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_INT unsigned_long
#define T_INT_MIN 0u
#define T_INT_MAX ULONG_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_INT unsigned_long
#define T_INT_MIN 0u
#define T_INT_MAX ULONG_MAX
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"
