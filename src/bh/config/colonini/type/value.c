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

#include "bh/config/colonini/type/value.h"

#include <assert.h>
#include <stddef.h>
#include <windows.h>

#include "bh/config/colonini/type/data.h"
#include "bh/config/colonini/type/map.h"
#include "bh/config/colonini/type/value_type.h"

/**
 * External
 */

struct Colonini_Value* Colonini_Value_InitAsBoolean(
    struct Colonini_Value* value, unsigned char data) {
  struct Colonini_Data* data_init_result;

  data_init_result =
      Colonini_Data_InitAsBoolean(&value->variant.as_data, data);
  if (data_init_result == NULL) {
    goto error;
  }
  value->type = Colonini_ValueType_kData;
  
  return value;

error:
  return NULL;
}

struct Colonini_Value* Colonini_Value_InitAsInteger(
    struct Colonini_Value* value, unsigned int data) {
  struct Colonini_Data* data_init_result;

  data_init_result =
      Colonini_Data_InitAsInteger(&value->variant.as_data, data);
  if (data_init_result == NULL) {
    goto error;
  }
  value->type = Colonini_ValueType_kData;
  
  return value;

error:
  return NULL;
}

struct Colonini_Value* Colonini_Value_InitAsMap(struct Colonini_Value* value) {
  struct Colonini_Map* map_init_result;

  map_init_result = Colonini_Map_Init(&value->variant.as_map);
  if (map_init_result == NULL) {
    goto error;
  }
  value->type = Colonini_ValueType_kMap;
  
  return value;

error:
  return NULL;
}

struct Colonini_Value* Colonini_Value_InitAsString(
    struct Colonini_Value* value, const char* str, size_t str_length) {
  struct Colonini_Data* data_init_result;

  data_init_result =
      Colonini_Data_InitAsString(&value->variant.as_data, str, str_length);
  if (data_init_result == NULL) {
    goto error;
  }
  value->type = Colonini_ValueType_kData;
  
  return value;

error:
  return NULL;
}

struct Colonini_Value* Colonini_Value_InitAsToggle(
    struct Colonini_Value* value, unsigned char enabled, BYTE key_code) {
  struct Colonini_Data* data_init_result;

  data_init_result =
      Colonini_Data_InitAsToggle(&value->variant.as_data, enabled, key_code);
  if (data_init_result == NULL) {
    goto error;
  }
  value->type = Colonini_ValueType_kData;
  
  return value;

error:
  return NULL;
}

void Colonini_Value_Deinit(struct Colonini_Value* value) {
  switch (value->type) {
    case Colonini_ValueType_kData: {
      value->type = Colonini_ValueType_kUnspecified;
      Colonini_Data_Deinit(&value->variant.as_data);
      break;
    }

    case Colonini_ValueType_kMap: {
      value->type = Colonini_ValueType_kUnspecified;
      Colonini_Map_Deinit(&value->variant.as_map);
      break;
    }

    case Colonini_ValueType_kUnspecified:
    default: {
      assert(0 && "This should never happen.");
      return;
    }
  }
}
