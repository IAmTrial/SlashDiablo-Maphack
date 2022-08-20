/**
 * SlashDiablo Maphack
 * Copyright (C) SlashDiablo Community
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

#ifndef BH_CONFIG_CONFIG_TYPES_H_
#define BH_CONFIG_CONFIG_TYPES_H_

#include <optional>

namespace config {

struct KeyPress {
  DWORD code;
};

struct KeyToggle {
  DWORD code;
  bool is_toggled_on;
};

} // namespace config

#endif /* BH_CONFIG_CONFIG_TYPES_H_ */
