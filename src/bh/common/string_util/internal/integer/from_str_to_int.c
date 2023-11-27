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

#include <wchar.h>

#include "bh/common/preprocessor/template/typedef.h"

/**
 * External
 */

#define T_CHAR char
#define T_INT signed_char
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_INT signed_char
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_INT short
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_INT short
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_INT int
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_INT int
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_INT long
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_INT long
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_INT unsigned_char
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_INT unsigned_char
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_INT unsigned_short
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_INT unsigned_short
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_INT unsigned_int
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_INT unsigned_int
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR char
#define T_INT unsigned_long
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"

#define T_CHAR wchar_t
#define T_INT unsigned_long
#include "bh/common/string_util/internal/integer/from_str_to_int_template.h"
