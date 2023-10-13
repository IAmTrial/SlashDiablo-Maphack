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

#include "bh/common/string_util/bool.h"

#include <string.h>
#include <wchar.h>

#include "bh/common/string_util/ascii.h"

/**
 * External
 */

/**
 * Define Bool_FromTrueFalseStr
 */

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_TRUE_STR_LITERAL "on"
#define T_FALSE_STR_LITERAL "off"
#define T_TO_LOWER_STR_FUNC_NAME Ascii_ToLowerStr
#define T_MEMCMP_FUNC_NAME memcmp
#define T_FUNC_NAME Bool_FromOnOffStr
#include "bh/common/string_util/internal/bool/from_true_false_str_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_TRUE_STR_LITERAL "on"
#define T_FALSE_STR_LITERAL "off"
#define T_TO_LOWER_STR_FUNC_NAME Ascii_ToLowerWStr
#define T_MEMCMP_FUNC_NAME wmemcmp
#define T_FUNC_NAME Bool_FromOnOffWStr
#include "bh/common/string_util/internal/bool/from_true_false_str_template.h"

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_TRUE_STR_LITERAL "true"
#define T_FALSE_STR_LITERAL "false"
#define T_TO_LOWER_STR_FUNC_NAME Ascii_ToLowerStr
#define T_MEMCMP_FUNC_NAME memcmp
#define T_FUNC_NAME Bool_FromTrueFalseStr
#include "bh/common/string_util/internal/bool/from_true_false_str_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_TRUE_STR_LITERAL "true"
#define T_FALSE_STR_LITERAL "false"
#define T_TO_LOWER_STR_FUNC_NAME Ascii_ToLowerWStr
#define T_MEMCMP_FUNC_NAME wmemcmp
#define T_FUNC_NAME Bool_FromTrueFalseWStr
#include "bh/common/string_util/internal/bool/from_true_false_str_template.h"

/**
 * Define Bool_ToTrueFalseStr
 */

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_TRUE_STR_LITERAL "on"
#define T_FALSE_STR_LITERAL "off"
#define T_FUNC_NAME Bool_ToOnOffStr
#include "bh/common/string_util/internal/bool/to_true_false_str_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_TRUE_STR_LITERAL "on"
#define T_FALSE_STR_LITERAL "off"
#define T_FUNC_NAME Bool_ToOnOffWStr
#include "bh/common/string_util/internal/bool/to_true_false_str_template.h"

#define T_CHAR char
#define T_STR_LITERAL_PREFIX
#define T_TRUE_STR_LITERAL "true"
#define T_FALSE_STR_LITERAL "false"
#define T_FUNC_NAME Bool_ToTrueFalseStr
#include "bh/common/string_util/internal/bool/to_true_false_str_template.h"

#define T_CHAR wchar_t
#define T_STR_LITERAL_PREFIX L
#define T_TRUE_STR_LITERAL "true"
#define T_FALSE_STR_LITERAL "false"
#define T_FUNC_NAME Bool_ToTrueFalseWStr
#include "bh/common/string_util/internal/bool/to_true_false_str_template.h"
