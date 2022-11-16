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

#ifndef BH_D2_D2LANG_FUNCTION_GET_LOCALE_TEXT_HPP_
#define BH_D2_D2LANG_FUNCTION_GET_LOCALE_TEXT_HPP_

#include <stdint.h>
#include <wchar.h>

namespace bh::d2::d2lang {

const wchar_t* GetLocaleText(unsigned short id);

}  // namespace bh::d2::d2lang

#endif  // BH_D2_D2LANG_FUNCTION_GET_LOCALE_TEXT_HPP_
