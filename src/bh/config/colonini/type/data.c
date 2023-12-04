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

#include "bh/config/colonini/type/data.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "bh/config/colonini/type/data_type.h"
#include "bh/config/colonini/type/string.h"
#include "bh/config/colonini/type/toggle.h"

/**
 * External
 */

struct Colonini_Data* Colonini_Data_InitAsBoolean(
    struct Colonini_Data* data, unsigned char value) {
  data->variant.as_boolean = value;
  data->type = Colonini_DataType_kBoolean;

  return data;
}

struct Colonini_Data* Colonini_Data_InitAsInteger(
    struct Colonini_Data* data, unsigned int value) {
  data->variant.as_integer = value;
  data->type = Colonini_DataType_kInteger;

  return data;
}

struct Colonini_Data* Colonini_Data_InitAsString(
    struct Colonini_Data* data, const char* str, size_t str_length) {
  struct Colonini_String* as_string_init_result;
  struct Colonini_String* as_string_concat_result;

  as_string_init_result =
      Colonini_String_Init(&data->variant.as_string, str_length);
  if (as_string_init_result == NULL) {
    goto error;
  }

  as_string_concat_result =
      Colonini_String_Concat(&data->variant.as_string, str, str_length);
  if (as_string_concat_result == NULL) {
    goto error_deinit_str;
  }
  data->type = Colonini_DataType_kString;

  return data;

error_deinit_str:
  Colonini_String_Deinit(&data->variant.as_string);

error:
  return NULL;
}

struct Colonini_Data* Colonini_Data_InitAsToggle(
    struct Colonini_Data* data, int enabled, BYTE key_code) {
  struct Colonini_Toggle* toggle_init_result;

  toggle_init_result =
      Colonini_Toggle_Init(&data->variant.as_toggle, enabled, key_code);
  if (toggle_init_result == NULL) {
    goto error;
  }
  data->type = Colonini_DataType_kToggle;

  return data;

error:
  return NULL;
}

void Colonini_Data_Deinit(struct Colonini_Data* data) {
  switch (data->type) {
    case Colonini_DataType_kBoolean: {
      data->type = Colonini_DataType_kUnspecified;
      data->variant.as_boolean = 0;
      break;
    }

    case Colonini_DataType_kInteger: {
      data->type = Colonini_DataType_kUnspecified;
      data->variant.as_integer = 0;
      break;
    }

    case Colonini_DataType_kString: {
      data->type = Colonini_DataType_kUnspecified;
      Colonini_String_Deinit(&data->variant.as_string);
      break;
    }

    case Colonini_DataType_kToggle: {
      data->type = Colonini_DataType_kUnspecified;
      Colonini_Toggle_Deinit(&data->variant.as_toggle);
      break;
    }

    case Colonini_DataType_kUnspecified:
    default: {
      assert(0 && "This should never happen.");
      return;
    }
  }
}
