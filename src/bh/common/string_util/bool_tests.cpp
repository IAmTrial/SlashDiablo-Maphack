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

#include "bh/common/string_util/bool.hpp"

#include <uchar.h>
#include <wchar.h>

#include <string>
#include <string_view>

namespace bh::common::string_util {

static_assert(!ToBool("").has_value());
static_assert(!ToBool("none").has_value());
static_assert(!ToBool("n/a").has_value());
static_assert(!ToBool("null").has_value());
static_assert(!ToBool("not").has_value());
static_assert(!ToBool("truue").has_value());
static_assert(!ToBool("fasle").has_value());
static_assert(!ToBool("yes").has_value());
static_assert(!ToBool("no").has_value());
static_assert(!ToBool("y").has_value());
static_assert(!ToBool("n").has_value());
static_assert(!ToBool("1").has_value());
static_assert(!ToBool("0").has_value());
static_assert(!ToBool("123456789").has_value());

static_assert(ToBool("true").value());
static_assert(ToBool("True").value());
static_assert(ToBool("TrUe").value());
static_assert(!ToBool("true ").has_value());
static_assert(!ToBool(" true").has_value());
static_assert(!ToBool(" true ").has_value());

static_assert(!ToBool("false").value());
static_assert(!ToBool("False").value());
static_assert(!ToBool("FaLsE").value());
static_assert(!ToBool(" false").has_value());
static_assert(!ToBool("false ").has_value());
static_assert(!ToBool(" false ").has_value());

static_assert(ToBool(std::string_view("true")).value());
static_assert(ToBool(std::string_view("True")).value());
static_assert(!ToBool(std::string_view("false")).value());
static_assert(!ToBool(std::string_view("False")).value());

static_assert(ToBool(L"true").value());
static_assert(ToBool(L"True").value());
static_assert(!ToBool(L"false").value());
static_assert(!ToBool(L"False").value());

static_assert(ToBool(u8"true").value());
static_assert(ToBool(u8"True").value());
static_assert(!ToBool(u8"false").value());
static_assert(!ToBool(u8"False").value());

static_assert(ToBool(u"true").value());
static_assert(ToBool(u"True").value());
static_assert(!ToBool(u"false").value());
static_assert(!ToBool(u"False").value());

static_assert(ToBool(U"true").value());
static_assert(ToBool(U"True").value());
static_assert(!ToBool(U"false").value());
static_assert(!ToBool(U"False").value());

}  // namespace bh::common::string_util