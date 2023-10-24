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
#include "bh/common/string_util/internal/bool/from_true_false_str.h"
#include "bh/common/string_util/internal/bool/to_true_false_str.h"

/**
 * External
 */

int* Bool_FromOnOffStr(int* dest, const char* src, size_t length) {
  return T_Bool_FromOnOffStr(char)(dest, src, length);
}

int* Bool_FromOnOffWStr(int* dest, const wchar_t* src, size_t length) {
  return T_Bool_FromOnOffStr(wchar_t)(dest, src, length);
}

int* Bool_FromTrueFalseStr(int* dest, const char* src, size_t length) {
  return T_Bool_FromTrueFalseStr(char)(dest, src, length);
}

int* Bool_FromTrueFalseWStr(int* dest, const wchar_t* src, size_t length) {
  return T_Bool_FromTrueFalseStr(wchar_t)(dest, src, length);
}

const char* Bool_ToOnOffStr(int value, size_t* length) {
  return T_Bool_ToOnOffStr(char)(value, length);
}

const wchar_t* Bool_ToOnOffWStr(int value, size_t* length) {
  return T_Bool_ToOnOffStr(wchar_t)(value, length);
}

const char* Bool_ToTrueFalseStr(int value, size_t* length) {
  return T_Bool_ToTrueFalseStr(char)(value, length);
}

const wchar_t* Bool_ToTrueFalseWStr(int value, size_t* length) {
  return T_Bool_ToTrueFalseStr(wchar_t)(value, length);
}
