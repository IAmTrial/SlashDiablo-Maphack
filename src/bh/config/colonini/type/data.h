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

#ifndef BH_CONFIG_COLONINI_TYPE_DATA_H_
#define BH_CONFIG_COLONINI_TYPE_DATA_H_

#include <stddef.h>
#include <windows.h>

#include "bh/config/colonini/type/data_type.h"
#include "bh/config/colonini/type/string.h"
#include "bh/config/colonini/type/toggle.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct Colonini_Data {
  enum Colonini_DataType type;
  union {
    unsigned char as_boolean;
    unsigned int as_integer;
    struct Colonini_String as_string;
    struct Colonini_Toggle as_toggle;
  } variant;
};

struct Colonini_Data* Colonini_Data_InitAsBoolean(
    struct Colonini_Data* data, unsigned char value);

struct Colonini_Data* Colonini_Data_InitAsInteger(
    struct Colonini_Data* data, unsigned int value);

struct Colonini_Data* Colonini_Data_InitAsString(
    struct Colonini_Data* data, const char* str, size_t str_length);

struct Colonini_Data* Colonini_Data_InitAsToggle(
    struct Colonini_Data* data, int enabled, BYTE key_code);

void Colonini_Data_Deinit(struct Colonini_Data* data);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_TYPE_DATA_H_ */
