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

#ifndef BH_CONFIG_COLONINI_TYPE_MAP_H_
#define BH_CONFIG_COLONINI_TYPE_MAP_H_

#include <stddef.h>
#include <windows.h>

#include "bh/common/data_struct/red_black_tree.h"
/*
 * entry.h must be not be included, otherwise there will be a circular
 * dependency.
 */
#if 0
#include "bh/config/colonini/type/entry.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct Colonini_Entry;

struct Colonini_Map {
  size_t count;
  struct Colonini_Entry* head_entry;
  struct Colonini_Entry* tail_entry;
  struct RedBlackTree tree;
};

struct Colonini_Map* Colonini_Map_Init(struct Colonini_Map* map);

void Colonini_Map_Deinit(struct Colonini_Map* map);

struct Colonini_Entry* Colonini_Map_Find(
    const struct Colonini_Map* map, const char* key, size_t key_length);

struct Colonini_Value* Colonini_Map_Get(
    const struct Colonini_Map* map, const char* key, size_t key_length);

unsigned char* Colonini_Map_PutBoolean(
    struct Colonini_Map* map,
    const char* key,
    size_t key_length,
    unsigned char value);

unsigned int* Colonini_Map_PutInteger(
    struct Colonini_Map* map,
    const char* key,
    size_t key_length,
    unsigned int value);

struct Colonini_Map* Colonini_Map_PutMap(
    struct Colonini_Map* map, const char* key, size_t key_length);

struct Colonini_String* Colonini_Map_PutString(
    struct Colonini_Map* map,
    const char* key,
    size_t key_length,
    const char* str,
    size_t str_length);

struct Colonini_Toggle* Colonini_Map_PutToggle(
    struct Colonini_Map* map,
    const char* key,
    size_t key_length,
    unsigned char enabled,
    BYTE key_code);

int Colonini_Map_Remove(
    struct Colonini_Map* map, const char* key, size_t key_length);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_TYPE_MAP_H_ */
