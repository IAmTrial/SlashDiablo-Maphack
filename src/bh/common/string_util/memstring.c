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

#include "bh/common/string_util/memstring.h"

#include <stddef.h>
#include <string.h>
#include <wchar.h>

/**
 * External
 */

/**
 * Define memcrspn
 */

#define T_CHAR char
#define T_INPUT void
#define T_FUNC_NAME MemCRSpn
#include "bh/common/string_util/internal/memstring/memcrspn_template.h"

#define T_CHAR wchar_t
#define T_INPUT wchar_t
#define T_FUNC_NAME WMemCRSpn
#include "bh/common/string_util/internal/memstring/memcrspn_template.h"

/**
 * Define memcspn
 */

#define T_CHAR char
#define T_INPUT void
#define T_FUNC_NAME MemCSpn
#include "bh/common/string_util/internal/memstring/memcspn_template.h"

#define T_CHAR wchar_t
#define T_INPUT wchar_t
#define T_FUNC_NAME WMemCSpn
#include "bh/common/string_util/internal/memstring/memcspn_template.h"

/**
 * Define memrspn
 */

#define T_CHAR char
#define T_INPUT void
#define T_FUNC_NAME MemRSpn
#include "bh/common/string_util/internal/memstring/memrspn_template.h"

#define T_CHAR wchar_t
#define T_INPUT wchar_t
#define T_FUNC_NAME WMemRSpn
#include "bh/common/string_util/internal/memstring/memrspn_template.h"

/**
 * Define memspn
 */

#define T_CHAR char
#define T_INPUT void
#define T_FUNC_NAME MemSpn
#include "bh/common/string_util/internal/memstring/memspn_template.h"

#define T_CHAR wchar_t
#define T_INPUT wchar_t
#define T_FUNC_NAME WMemSpn
#include "bh/common/string_util/internal/memstring/memspn_template.h"

/**
 * Define memstr
 */

#define T_CHAR char
#define T_INPUT void
#define T_MEMCHR_FUNC_NAME memchr
#define T_MEMCMP_FUNC_NAME memcmp
#define T_FUNC_NAME MemStr
#include "bh/common/string_util/internal/memstring/memstr_template.h"

#define T_CHAR wchar_t
#define T_INPUT wchar_t
#define T_MEMCHR_FUNC_NAME wmemchr
#define T_MEMCMP_FUNC_NAME wmemcmp
#define T_FUNC_NAME WMemStr
#include "bh/common/string_util/internal/memstring/memstr_template.h"
