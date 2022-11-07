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

#include "bh/common/string_util/trim.hpp"

#include <uchar.h>
#include <wchar.h>

#include <string>
#include <string_view>

namespace bh::common::string_util {

static_assert(Trim("") == "");
static_assert(Trim(" ") == "");
static_assert(Trim("\t") == "");
static_assert(Trim("\n") == "");
static_assert(Trim("\r") == "");
static_assert(Trim("\r\n") == "");
static_assert(Trim(" \r\n") == "");

static_assert(Trim("test") == "test");
static_assert(Trim(" test") == "test");
static_assert(Trim("test ") == "test");
static_assert(Trim(" test ") == "test");
static_assert(Trim("\ttest") == "test");
static_assert(Trim("test\t") == "test");
static_assert(Trim("\ttest\t") == "test");
static_assert(Trim("\r\ntest") == "test");
static_assert(Trim("test\r\n") == "test");
static_assert(Trim("\r\ntest\r\n") == "test");

static_assert(Trim(std::string_view("test")) == "test");
static_assert(Trim(std::string_view(" test")) == "test");
static_assert(Trim(std::string_view("test ")) == "test");
static_assert(Trim(std::string_view(" test ")) == "test");
static_assert(Trim(std::string_view("\ttest")) == "test");
static_assert(Trim(std::string_view("test\t")) == "test");
static_assert(Trim(std::string_view("\ttest\t")) == "test");
static_assert(Trim(std::string_view("\r\ntest")) == "test");
static_assert(Trim(std::string_view("test\r\n")) == "test");
static_assert(Trim(std::string_view("\r\ntest\r\n")) == "test");

static_assert(Trim(L"test") == L"test");
static_assert(Trim(L" test") == L"test");
static_assert(Trim(L"test ") == L"test");
static_assert(Trim(L" test ") == L"test");
static_assert(Trim(L"\ttest") == L"test");
static_assert(Trim(L"test\t") == L"test");
static_assert(Trim(L"\ttest\t") == L"test");
static_assert(Trim(L"\r\ntest") == L"test");
static_assert(Trim(L"test\r\n") == L"test");
static_assert(Trim(L"\r\ntest\r\n") == L"test");

static_assert(Trim(u8"test") == u8"test");
static_assert(Trim(u8" test") == u8"test");
static_assert(Trim(u8"test ") == u8"test");
static_assert(Trim(u8" test ") == u8"test");
static_assert(Trim(u8"\ttest") == u8"test");
static_assert(Trim(u8"test\t") == u8"test");
static_assert(Trim(u8"\ttest\t") == u8"test");
static_assert(Trim(u8"\r\ntest") == u8"test");
static_assert(Trim(u8"test\r\n") == u8"test");
static_assert(Trim(u8"\r\ntest\r\n") == u8"test");

static_assert(Trim(u"test") == u"test");
static_assert(Trim(u" test") == u"test");
static_assert(Trim(u"test ") == u"test");
static_assert(Trim(u" test ") == u"test");
static_assert(Trim(u"\ttest") == u"test");
static_assert(Trim(u"test\t") == u"test");
static_assert(Trim(u"\ttest\t") == u"test");
static_assert(Trim(u"\r\ntest") == u"test");
static_assert(Trim(u"test\r\n") == u"test");
static_assert(Trim(u"\r\ntest\r\n") == u"test");

static_assert(Trim(U"test") == U"test");
static_assert(Trim(U" test") == U"test");
static_assert(Trim(U"test ") == U"test");
static_assert(Trim(U" test ") == U"test");
static_assert(Trim(U"\ttest") == U"test");
static_assert(Trim(U"test\t") == U"test");
static_assert(Trim(U"\ttest\t") == U"test");
static_assert(Trim(U"\r\ntest") == U"test");
static_assert(Trim(U"test\r\n") == U"test");
static_assert(Trim(U"\r\ntest\r\n") == U"test");

}  // namespace bh::common::string_util