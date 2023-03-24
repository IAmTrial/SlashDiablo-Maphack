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

#ifndef BH_CONFIG_COLONINI_TYPE_KEY_H_
#define BH_CONFIG_COLONINI_TYPE_KEY_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

enum Colonini_KeyType {
  Colonini_KeyType_kUnspecified = 0,

  Colonini_KeyType_kString,
  Colonini_KeyType_kIndex
};

struct Colonini_Key {
  enum Colonini_KeyType type;
  union {
    char* as_string;
    size_t as_index;
  } variant;
};

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_TYPE_KEY_H_ */
