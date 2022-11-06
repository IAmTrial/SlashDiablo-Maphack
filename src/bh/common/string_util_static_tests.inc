/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2022  SlashDiablo Community
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

#if !defined(BH_COMMON_STRING_UTIL_HPP_)
#error This file is not to be directly included by external users.
#endif  // !defined(BH_COMMON_STRING_UTIL_HPP_)

#if !defined(BH_COMMON_STRING_UTIL_TEMPLATE_INC_)
#error Constexpr functions must be defined before including the static tests.
#endif  // !defined(BH_COMMON_STRING_UTIL_TEMPLATE_INC_)

#ifndef BH_COMMON_STRING_UTIL_STATIC_TEST_INC_
#define BH_COMMON_STRING_UTIL_STATIC_TEST_INC_

#include <limits>

namespace common::str_util {

static_assert(!ToIntegerFromDigit<int>('0', 0));
static_assert(!ToIntegerFromDigit<int>(L'0', 0));
static_assert(!ToIntegerFromDigit<int>('0', 37));
static_assert(!ToIntegerFromDigit<int>('.', 36));
static_assert(!ToIntegerFromDigit<int>('/', 36));
static_assert(!ToIntegerFromDigit<int>('\xFF', 36));
static_assert(ToIntegerFromDigit<int>('z', 36) == 35);
static_assert(ToIntegerFromDigit<int>('G', 36) == 16);
static_assert(!ToIntegerFromDigit<int>('.', 36));
static_assert(ToIntegerFromDigit<int>('Y', 35) == 34);
static_assert(ToIntegerFromDigit<int>('g', 35) == 16);
static_assert(!ToIntegerFromDigit<int>('Z', 35));
static_assert(ToIntegerFromDigit<int>('f', 16) == 15);
static_assert(ToIntegerFromDigit<int>('A', 16) == 10);
static_assert(!ToIntegerFromDigit<int>('i', 16));
static_assert(ToIntegerFromDigit<int>('9', 10) == 9);
static_assert(ToIntegerFromDigit<int>('8', 10) == 8);
static_assert(!ToIntegerFromDigit<int>('A', 10));
static_assert(ToIntegerFromDigit<int>('7', 8) == 7);
static_assert(ToIntegerFromDigit<int>('1', 8) == 1);
static_assert(!ToIntegerFromDigit<int>('8', 8));

static_assert(GetIntegerBaseFromPrefix("0xa") == 16);
static_assert(GetIntegerBaseFromPrefix(L"0xa") == 16);
static_assert(GetIntegerBaseFromPrefix("-0xa") == 16);
static_assert(GetIntegerBaseFromPrefix("0xF") == 16);
static_assert(GetIntegerBaseFromPrefix("-0xF") == 16);
static_assert(GetIntegerBaseFromPrefix("18") == 10);
static_assert(GetIntegerBaseFromPrefix("07") == 8);
static_assert(GetIntegerBaseFromPrefix("0x7F") == 16);
static_assert(GetIntegerBaseFromPrefix("-0x7F") == 16);

static_assert(GetIntegerBaseFromPrefix("0xz") == 16);
static_assert(!GetIntegerBaseFromPrefix("f"));
static_assert(!GetIntegerBaseFromPrefix("X"));
static_assert(!GetIntegerBaseFromPrefix("a"));
static_assert(GetIntegerBaseFromPrefix("08") == 8);
static_assert(GetIntegerBaseFromPrefix("99999999999999999999") == 10);
static_assert(GetIntegerBaseFromPrefix("257") == 10);
static_assert(GetIntegerBaseFromPrefix("0x100") == 16);
static_assert(GetIntegerBaseFromPrefix("0x") == 16);
static_assert(!GetIntegerBaseFromPrefix(""));
static_assert(GetIntegerBaseFromPrefix("0X") == 16);
static_assert(GetIntegerBaseFromPrefix("0x80") == 16);

static_assert(ToInteger<int>("0xa") == 10);
static_assert(ToInteger<int>(L"0xa") == 10);
static_assert(ToInteger<int>("-0xa") == -10);
static_assert(ToInteger<int>("0xF") == 15);
static_assert(ToInteger<int>("-0xF") == -15);
static_assert(ToInteger<int>("18") == 18);
static_assert(ToInteger<int>("07") == 7);
static_assert(
    ToInteger<unsigned int>("0xFFffFFff")
        == std::numeric_limits<unsigned int>::max());
static_assert(ToInteger<signed char>("0x7F") == 127);
static_assert(ToInteger<signed char>("-0x7F") == -127);
static_assert(ToInteger<signed char>("-0x80") == -128);
static_assert(
    ToInteger<unsigned char>("0xfF")
        == std::numeric_limits<unsigned char>::max());

static_assert(!ToInteger<int>("0xz"));
static_assert(!ToInteger<int>("f"));
static_assert(!ToInteger<int>("X"));
static_assert(!ToInteger<int>("a"));
static_assert(!ToInteger<int>("08"));
static_assert(!ToInteger<signed char>("0x80"));
static_assert(!ToInteger<int>("0xfffffffF"));
static_assert(!ToInteger<int>("99999999999999999999999999"));
static_assert(!ToInteger<unsigned char>("257"));
static_assert(!ToInteger<unsigned char>("0x100"));
static_assert(!ToInteger<int>("0x"));
static_assert(!ToInteger<int>(""));
static_assert(!ToInteger<int>("0X"));
static_assert(!ToInteger<signed char>("0x80"));

static_assert(IsDecimal<unsigned char>("0"));
static_assert(IsDecimal<unsigned char>(L"0"));
static_assert(IsDecimal<unsigned char>("255"));
static_assert(!IsDecimal<unsigned char>("256"));
static_assert(!IsDecimal<unsigned char>(""));
static_assert(!IsDecimal<unsigned char>("\t"));
static_assert(!IsDecimal<unsigned char>("Hello world!"));
static_assert(!IsDecimal<unsigned char>("0x0"));
static_assert(!IsDecimal<unsigned char>("0xFF"));
static_assert(!IsDecimal<unsigned char>("010"));
static_assert(!IsDecimal<unsigned char>("00"));
static_assert(!IsDecimal<unsigned char>("07"));

static_assert(IsHex<unsigned char>("0x0"));
static_assert(IsHex<unsigned char>(L"0x0"));
static_assert(IsHex<unsigned char>("0x00"));
static_assert(IsHex<unsigned char>("0xFF"));
static_assert(!IsHex<unsigned char>("0x100"));
static_assert(IsHex<unsigned int>("0x100"));
static_assert(IsHex<unsigned char>("0x00000000FF"));
static_assert(!IsHex<unsigned char>("0xG"));
static_assert(!IsHex<unsigned char>(""));
static_assert(!IsHex<unsigned char>("\t"));
static_assert(!IsHex<unsigned char>("Hello world!"));
static_assert(!IsHex<unsigned char>("0"));
static_assert(!IsHex<unsigned char>("10"));
static_assert(!IsHex<unsigned char>("01"));
static_assert(!IsHex<unsigned char>("07"));

static_assert(IsOctal<unsigned char>("00"));
static_assert(IsOctal<unsigned char>(L"00"));
static_assert(IsOctal<unsigned char>("01"));
static_assert(IsOctal<unsigned char>("0377"));
static_assert(IsOctal<unsigned char>("000000000377"));
static_assert(!IsOctal<unsigned char>("0400"));
static_assert(IsOctal<unsigned int>("0400"));
static_assert(!IsOctal<unsigned char>("08"));
static_assert(!IsOctal<unsigned char>(""));
static_assert(!IsOctal<unsigned char>("\t"));
static_assert(!IsOctal<unsigned char>("Hello world!"));
static_assert(!IsOctal<unsigned char>("0"));
static_assert(!IsOctal<unsigned char>("0x10"));
static_assert(!IsOctal<unsigned char>("0x01"));
static_assert(!IsOctal<unsigned char>("0x07"));
static_assert(!IsOctal<unsigned char>("1"));
static_assert(!IsOctal<unsigned char>("7"));

}  // namespace common::str_util

#endif  // BH_COMMON_STRING_UTIL_STATIC_TEST_INC_
