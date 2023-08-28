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

#include "bh/config/colonini/internal/semantic_analyzer/typing.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "bh/config/colonini/internal/parser/assign_statement.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/const_expr_type.h"
#include "bh/config/colonini/internal/parser/key_expr.h"
#include "bh/config/colonini/internal/parser/parser_line.h"
#include "bh/config/colonini/internal/parser/value_expr.h"
#include "bh/config/colonini/internal/parser/value_expr_type.h"

/**
 * External
 */

struct Typing* Typing_Init(
    struct Typing* typing,
    const struct KeyExpr* keys,
    const struct ValueExpr* value) {
  typing->key_name = &keys->constexpr;
  typing->subkey_count = keys->subscripts_count;

  typing->value_type = value->type;
  typing->value_as_constexpr_type =
      (typing->value_type == ValueExprType_kConst)
          ? value->variant.as_constexpr.type
          : ConstExprType_kUnspecified;

  return typing;
}

void Typing_Deinit(struct Typing* typing) {
  typing->value_as_constexpr_type = ConstExprType_kUnspecified;
  typing->value_type = ValueExprType_kUnspecified;
  typing->subkey_count = 0;
  typing->key_name = NULL;
}

int Typing_CompareKeyName(
    const struct Typing* left, const struct Typing* right) {
  return ConstExpr_CompareExprAsString(left->key_name, right->key_name);
}

int Typing_Equal(const struct Typing* left, const struct Typing* right) {
  size_t i;

  if (left->subkey_count != right->subkey_count) {
    return 0;
  }

  if (!ConstExpr_Equal(left->key_name, right->key_name)) {
    return 0;
  }

  if (left->value_type != right->value_type) {
    return 0;
  }

  if (left->value_type == ValueExprType_kConst
      && left->value_as_constexpr_type != right->value_as_constexpr_type) {
    return 0;
  }

  return 1;
}

int Typing_ResolveDiff(struct Typing* left, struct Typing* right) {
  size_t i;

  /* Cannot resolve if subkey count is different. */
  if (left->subkey_count != right->subkey_count) {
    return 0;
  }

  /* String is the only type that is compatible with all other types. */
  if (left->value_type != right->value_type) {
    left->value_type = ValueExprType_kConst;
    right->value_type = ValueExprType_kConst;
    left->value_as_constexpr_type = ConstExprType_kString;
    right->value_as_constexpr_type = ConstExprType_kString;

    return 1;
  }

  if (left->value_type == ValueExprType_kToggle) {
    return 1;
  }

  if (left->value_as_constexpr_type == right->value_as_constexpr_type) {
    return 1;
  }

  /* Signed int and unsigned int are compatible. */
  if (left->value_as_constexpr_type == ConstExprType_kSignedInt
      && right->value_as_constexpr_type == ConstExprType_kUnsignedInt) {
    return 1;
  }
  if (left->value_as_constexpr_type == ConstExprType_kUnsignedInt
      && right->value_as_constexpr_type == ConstExprType_kSignedInt) {
    return 1;
  }

  left->value_as_constexpr_type = ConstExprType_kString;
  right->value_as_constexpr_type = ConstExprType_kString;

  return 1;
}

int Typing_ResolveDiffFromKeysAndValue(
    struct Typing* typing,
    const struct KeyExpr* keys,
    const struct ValueExpr* value) {
  struct Typing* temp_init_result;

  struct Typing temp;
  int resolve_succeeded;

  temp_init_result = Typing_Init(&temp, keys, value);
  if (temp_init_result == NULL) {
    goto error;
  }

  resolve_succeeded = Typing_ResolveDiff(typing, &temp);

  Typing_Deinit(&temp);

  return resolve_succeeded;

error:
  return 0;
}
