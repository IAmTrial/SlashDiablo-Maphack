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

#include <stddef.h>

#include "bh/config/colonini/type/data.h"
#include "bh/config/colonini/type/map.h"
#include "bh/config/colonini/type/value_type.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct Colonini_Value {
  enum Colonini_ValueType type;
  union {
    struct Colonini_Data as_data;
    struct Colonini_Map as_map;
  } variant;
};

struct Colonini_Value* Colonini_Value_InitAsBoolean(
    struct Colonini_Value* value, unsigned char data);

struct Colonini_Value* Colonini_Value_InitAsInteger(
    struct Colonini_Value* value, unsigned int data);

struct Colonini_Value* Colonini_Value_InitAsMap(struct Colonini_Value* value);

struct Colonini_Value* Colonini_Value_InitAsString(
    struct Colonini_Value* value, const char* str, size_t str_length);

void Colonini_Value_Deinit(struct Colonini_Value* value);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_TYPE_VALUE_H_ */
