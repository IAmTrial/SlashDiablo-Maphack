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

#include "bh/config/colonini/internal/semantic_analyzer/variable.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "bh/config/colonini/internal/parser.h"
#include "bh/config/colonini/internal/parser/assign_statement.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/const_expr_type.h"
#include "bh/config/colonini/internal/parser/key_expr.h"
#include "bh/config/colonini/internal/parser/toggle_expr.h"
#include "bh/config/colonini/internal/parser/value_expr.h"
#include "bh/config/colonini/internal/parser/value_expr_type.h"

struct Variable* Variable_Init(
    struct Variable* variable,
    const struct AssignStatement* assign_statement) {
  const struct KeyExpr* keys;
  const struct ConstExpr* primary_key;
  const struct ValueExpr* value;

  keys = &assign_statement->key_expr;
  primary_key = &keys->constexpr;

  /* Copy the primary key name. */
  variable->name =
      malloc((primary_key->length + 1) * sizeof(variable->name[0]));
  if (variable->name == NULL) {
    goto error;
  }
  memcpy(variable->name, primary_key->expr, primary_key->length);
  variable->name[primary_key->length] = '\0';
  variable->name_length = primary_key->length;

  /* Copy the subkeys' types */
  variable->subkey_types =
      malloc(keys->subscripts_count * sizeof(variable->subkey_types[0]));
  if (variable->subkey_types == NULL) {
    goto error_free_name;
  }

  for (variable->subkey_types_count = 0;
      variable->subkey_types_count < keys->subscripts_count;
      ++variable->subkey_types_count) {
    variable->subkey_types[variable->subkey_types_count] =
        keys->subscripts[variable->subkey_types_count].expr.type;
  }

  /* Copy the value's type. */
  value = &assign_statement->value_expr;
  variable->value_type = value->type;
  variable->value_as_constexpr_type =
      (variable->value_type == ValueExprType_kConst)
          ? value->variant.as_constexpr.type
          : ConstExprType_kUnspecified;

  return variable;

error_free_name:
  variable->name_length = 0;
  free(variable->name);
  variable->name = NULL;

error:
  return NULL;
}

void Variable_Deinit(struct Variable* variable) {
  variable->value_as_constexpr_type = ConstExprType_kUnspecified;
  variable->value_type = ValueExprType_kUnspecified;

  free(variable->subkey_types);
  variable->subkey_types_count = 0;
  variable->subkey_types = NULL;

  variable->name_length = 0;
  free(variable->name);
  variable->name = NULL;
}

int Variable_CompareName(
    const struct Variable* left, const struct Variable* right) {
  int is_lengths_same;
  int is_left_shorter;
  const struct Variable* shorter;
  int name_compare_result;

  is_lengths_same = (left->name_length == right->name_length);
  if (is_lengths_same) {
    return memcmp(left->name, right->name, left->name_length);
  }

  is_left_shorter = (left->name_length < right->name_length);
  shorter = (is_left_shorter) ? left : right;

  name_compare_result =
      memcmp(left->name, right->name, shorter->name_length);
  if (name_compare_result == 0) {
    return (is_left_shorter) ? -1 : 1;
  }

  return name_compare_result;
}

int Variable_EqualLineTypes(
    const struct Variable* left, const struct ParserLine* right) {
  size_t i;
  const struct AssignStatement* assign_statement;
  const struct KeyExpr* right_key;
  const struct ValueExpr* right_value;

  assign_statement = &right->variant.assign_statement;
  right_key = &assign_statement->key_expr;
  if (left->subkey_types_count != right_key->subscripts_count) {
    return 0;
  }

  for (i = 0; i < left->subkey_types_count; ++i) {
    if (left->subkey_types[i] != right_key->subscripts[i].expr.type) {
      return 0;
    }
  }

  right_value = &assign_statement->value_expr;
  if ((left->value_type != right_value->type)) {
    return 0;
  }

  if (left->value_type == ValueExprType_kConst) {
    const struct ConstExpr* right_value_as_constexpr;

    right_value_as_constexpr = &right_value->variant.as_constexpr;
    return (left->value_as_constexpr_type == right_value_as_constexpr->type);
  }

  return 1;
}

int Variable_ResolveLineTypeDifference(
    struct Variable* left, struct ParserLine* right) {
  size_t i;
  struct KeyExpr* right_key;
  struct ValueExpr* right_value;

  /* Cannot resolve if subkey count is different. */
  right_key = &right->variant.assign_statement.key_expr;
  if (left->subkey_types_count != right_key->subscripts_count) {
    return 0;
  }

  /* Resolve subkey types. */
  for (i = 0; i < left->subkey_types_count; ++i) {
    if (left->subkey_types[i] != right_key->subscripts[i].expr.type) {
      left->subkey_types[i] = ConstExprType_kString;
      right_key->subscripts[i].expr.type = ConstExprType_kString;
    }
  }

  /* Resolve value types. */
  right_value = &right->variant.assign_statement.value_expr;
  if ((left->value_type != right_value->type)) {
    left->value_type = ValueExprType_kConst;
    left->value_as_constexpr_type = ConstExprType_kString;

    /* Convert ToggleExpr to ConstExpr. */
    if (right_value->type == ValueExprType_kToggle) {
      struct ConstExpr temp_constexpr;
      struct ConstExpr* init_result;
      struct ToggleExpr* right_value_as_toggleexpr;

      right_value_as_toggleexpr = &right_value->variant.as_toggleexpr;
      init_result =
          ConstExpr_Init(
              &temp_constexpr,
              right_value_as_toggleexpr->enabled_expr.begin_src,
              right_value_as_toggleexpr->input_expr.end_src);
      if (init_result == NULL) {
        goto error;
      }

      ToggleExpr_Deinit(right_value_as_toggleexpr);
      right_value->type = ValueExprType_kConst;
      right_value->variant.as_constexpr = temp_constexpr;
    }
    return 1;
  }

  if (left->value_type == ValueExprType_kConst) {
    struct ConstExpr* right_value_as_constexpr;

    right_value_as_constexpr = &right_value->variant.as_constexpr;
    if (left->value_as_constexpr_type != right_value_as_constexpr->type) {
      left->value_as_constexpr_type = ConstExprType_kString;
      right_value_as_constexpr->type = ConstExprType_kString;
    }
  }

  return 1;

error:
  return 0;
}
