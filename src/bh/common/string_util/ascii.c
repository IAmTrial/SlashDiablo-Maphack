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

#include "bh/common/string_util/internal/ascii/to_lower_char.h"
#include "bh/common/string_util/internal/ascii/to_lower_str.h"
#include "bh/common/string_util/internal/ascii/to_upper_char.h"
#include "bh/common/string_util/internal/ascii/to_upper_str.h"
#include "bh/common/string_util/internal/ascii/trim_whitespace_chars.h"
#include "bh/common/string_util/memstring.h"

/**
 * External
 */

char Ascii_ToLowerChar(char ch) {
  return T_Ascii_ToLowerChar(char)(ch);
}

wchar_t Ascii_ToLowerWChar(wchar_t ch) {
  return T_Ascii_ToLowerChar(wchar_t)(ch);
}

char* Ascii_ToLowerStr(char* dest, const char* src, size_t length) {
  return T_Ascii_ToLowerStr(char)(dest, src, length);
}

wchar_t* Ascii_ToLowerWStr(wchar_t* dest, const wchar_t* src, size_t length) {
  return T_Ascii_ToLowerStr(wchar_t)(dest, src, length);
}

char Ascii_ToUpperChar(char ch) {
  return T_Ascii_ToUpperChar(char)(ch);
}

wchar_t Ascii_ToUpperWChar(wchar_t ch) {
  return T_Ascii_ToUpperChar(wchar_t)(ch);
}

char* Ascii_ToUpperStr(char* dest, const char* src, size_t length) {
  return T_Ascii_ToUpperStr(char)(dest, src, length);
}

wchar_t* Ascii_ToUpperWStr(wchar_t* dest, const wchar_t* src, size_t length) {
  return T_Ascii_ToUpperStr(wchar_t)(dest, src, length);
}

char* Ascii_TrimWhitespaceChars(char* dest, const char* src, size_t length) {
  return T_Ascii_TrimWhitespaceChars(char)(dest, src, length);
}

wchar_t* Ascii_TrimWhitespaceWChars(
    wchar_t* dest, const wchar_t* src, size_t length) {
  return T_Ascii_TrimWhitespaceChars(wchar_t)(dest, src, length);
}
