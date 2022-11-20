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

#ifndef BH_D2_D2WIN_FUNCTION_LOAD_MPQ_HPP_
#define BH_D2_D2WIN_FUNCTION_LOAD_MPQ_HPP_

#include <stdint.h>
#include <wchar.h>

#include "bh/d2/struct/mpq_handle.hpp"

namespace bh::d2::d2win {

::bh::d2::MpqHandle* LoadMpq(
    const char* dll_filename,
    const char* mpq_path,
    const char* mpq_name,
    bool is_set_err_on_drive_query_fail,
    void* (*on_fail_callback),
    int priority);

}  // namespace bh::d2::d2win

#endif  // BH_D2_D2WIN_FUNCTION_LOAD_MPQ_HPP_
