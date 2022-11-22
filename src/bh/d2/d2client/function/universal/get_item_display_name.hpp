/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2022  SlashDiablo Team
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

#ifndef BH_D2_D2CLIENT_FUNCTION_UNIVERSAL_GET_ITEM_DISPLAY_NAME_HPP_
#define BH_D2_D2CLIENT_FUNCTION_UNIVERSAL_GET_ITEM_DISPLAY_NAME_HPP_

#include "bh/d2/struct/universal/unit.hpp"

#include <wchar.h>

namespace bh::d2::d2client::universal {

void GetItemDisplayName(
    const ::bh::d2::universal::Unit* unit,
    wchar_t* display_name,
    int display_name_max_length);

}  // namespace bh::d2::d2client::universal

#endif  // BH_D2_D2CLIENT_FUNCTION_UNIVERSAL_GET_ITEM_DISPLAY_NAME_HPP_
