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

#ifndef BH_CONFIG_COLONINI_TYPE_VALUE_H_
#define BH_CONFIG_COLONINI_TYPE_VALUE_H_

#include "bh/config/colonini/type/data.h"
#include "bh/config/colonini/type/list.h"
#include "bh/config/colonini/type/map.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

enum Colonini_ValueType {
  Colonini_ValueType_kData,
  Colonini_ValueType_kList,
  Colonini_ValueType_kMap
};

struct Colonini_Value {
  enum Colonini_ValueType type;
  union {
    struct Colonini_Data as_data;
    struct Colonini_List as_list;
    struct Colonini_Map as_map;
  } variant;
};

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_TYPE_VALUE_H_ */
