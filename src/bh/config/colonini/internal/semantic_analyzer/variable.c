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
    struct Variable* variable, const struct ParserLine* line) {
  const struct AssignStatement* assign_statement;
  const struct KeyExpr* keys;
  const struct ConstExpr* primary_key;
  const struct ValueExpr* value;

  /* Copy the line number. */
  variable->line_number = line->line_number;

  assign_statement = &line->variant.assign_statement;
  keys = &assign_statement->key_expr;
  primary_key = &keys->constexpr;

  /* Copy the primary key name. */
  variable->key_expr = keys;

  /* Copy the subkeys' types */
  variable->subkey_types =
      malloc(keys->subscripts_count * sizeof(variable->subkey_types[0]));
  if (variable->subkey_types == NULL) {
    goto error;
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

error:
  return NULL;
}

void Variable_Deinit(struct Variable* variable) {
  variable->value_as_constexpr_type = ConstExprType_kUnspecified;
  variable->value_type = ValueExprType_kUnspecified;

  free(variable->subkey_types);
  variable->subkey_types_count = 0;
  variable->subkey_types = NULL;

  variable->key_expr = NULL;
  variable->line_number = 0;
}

int Variable_CompareName(
    const struct Variable* left, const struct Variable* right) {
  const struct ConstExpr* left_expr;
  const struct ConstExpr* right_expr;

  int is_lengths_same;
  int is_left_shorter;
  const struct ConstExpr* shorter_expr;
  int name_cmp_result;

  left_expr = &left->key_expr->constexpr;
  right_expr = &right->key_expr->constexpr;

  is_lengths_same = (left_expr->length == right_expr->length);
  if (is_lengths_same) {
    return memcmp(left_expr->expr, right_expr->expr, left_expr->length);
  }

  is_left_shorter = (left_expr->length < right_expr->length);
  shorter_expr = (is_left_shorter) ? left_expr : right_expr;

  name_cmp_result =
      memcmp(left_expr->expr, right_expr->expr, shorter_expr->length);
  if (name_cmp_result == 0) {
    return (is_left_shorter) ? -1 : 1;
  }

  return name_cmp_result;
}

int Variable_CompareNameAndLineNumber(
    const struct Variable* left, const struct Variable* right) {
  int name_cmp_result;

  name_cmp_result = Variable_CompareName(left, right);
  if (name_cmp_result == 0) {
    return left->line_number - right->line_number;
  }
  return name_cmp_result;
}

int Variable_EqualKeys(
    const struct Variable* left, const struct Variable* right) {
  return KeyExpr_Equal(left->key_expr, right->key_expr);
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

int Variable_EqualName(
    const struct Variable* left, const struct Variable* right) {
  return ConstExpr_Equal(
      &left->key_expr->constexpr, &right->key_expr->constexpr);
}

int Variable_ResolveTypesDiffs(
    struct Variable* left, struct Variable* right) {
  size_t i;
  int equal_result;

  /* Base variable name needs to be the same for a resolve to be valid. */
  if (!Variable_EqualName(left, right)) {
    return 0;
  }

  /* Subkey count needs to be the same to resolve subkey types. */
  if (left->subkey_types_count != right->subkey_types_count) {
    return 0;
  }

  for (i = 0; i < left->subkey_types_count; ++i) {
    if (left->subkey_types[i] != right->subkey_types[i]) {
      left->subkey_types[i] = ConstExprType_kString;
      right->subkey_types[i] = ConstExprType_kString;
    }
  }

  /* Resolve value types. */
  if (left->value_type != right->value_type) {
    left->value_type = ValueExprType_kConst;
    right->value_type = ValueExprType_kConst;
    left->value_as_constexpr_type = ConstExprType_kString;
    right->value_as_constexpr_type = ConstExprType_kString;
  } else if (left->value_type == ValueExprType_kConst
      && left->value_as_constexpr_type != right->value_as_constexpr_type) {
    left->value_as_constexpr_type = ConstExprType_kString;
    right->value_as_constexpr_type = ConstExprType_kString;
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
