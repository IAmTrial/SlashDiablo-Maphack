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

#ifndef BH_CONFIG_COLONINI_TYPE_STRING_H_
#define BH_CONFIG_COLONINI_TYPE_STRING_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct Colonini_String {
  /** UTF-8 encoded, zero-terminated string. */
  char* str;

  /**
   * The length of the string from the first code unit to unit before
   * the zero-terminator.
   */
  size_t length;
};

struct Colonini_String* Colonini_String_Init(
    struct Colonini_String* dest, const char* src, size_t src_length);

void Colonini_String_Deinit(struct Colonini_String* str);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_TYPE_STRING_H_ */
