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

#ifndef BH_CONFIG_COLONINI_TYPE_ENTRY_H_
#define BH_CONFIG_COLONINI_TYPE_ENTRY_H_

#include <stddef.h>

#include "bh/config/colonini/type/string.h"
#include "bh/config/colonini/type/value.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct Colonini_Entry {
  struct Colonini_String key;
  struct Colonini_Value value;

  struct Colonini_Entry* previous;
  struct Colonini_Entry* next;
};

struct Colonini_Entry* Colonini_Entry_InitEmpty(
    struct Colonini_Entry* entry,
    const char* key,
    size_t key_length);

void Colonini_Entry_Deinit(struct Colonini_Entry* entry);

int Colonini_Entry_CompareKey(
    const struct Colonini_Entry* left, const struct Colonini_Entry* right);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_TYPE_ENTRY_H_ */
