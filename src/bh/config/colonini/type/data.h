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

#ifndef BH_CONFIG_COLONINI_TYPE_DATA_H_
#define BH_CONFIG_COLONINI_TYPE_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

enum Colonini_DataType {
  Colonini_DataType_kUnspecified = 0,

  Colonini_DataType_kBoolean,
  Colonini_DataType_kInt,
  Colonini_DataType_kUInt,
  Colonini_DataType_kString
};

struct Colonini_Data {
  enum Colonini_DataType type;
  union {
    unsigned char as_bool;
    int as_int;
    unsigned int as_uint;
    char* as_string;
  } variant;
};

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_TYPE_DATA_H_ */
