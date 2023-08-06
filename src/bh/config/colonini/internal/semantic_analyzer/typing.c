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

static struct Typing* Typing_InitFromAssignStatement(
    struct Typing* typing, const struct AssignStatement* assign_statement) {
  const struct KeyExpr* keys;
  const struct ConstExpr* primary_key;
  const struct ValueExpr* value;

  /* Init the key typing info. */
  keys = &assign_statement->key_expr;
  typing->key_name = &keys->constexpr;

  /* Copy the subkeys' types */
  typing->subkey_types =
      malloc(keys->subscripts_count * sizeof(typing->subkey_types[0]));
  if (typing->subkey_types == NULL) {
    goto error;
  }
  for (typing->subkey_type_count = 0;
      typing->subkey_type_count < keys->subscripts_count;
      ++typing->subkey_type_count) {
    typing->subkey_types[typing->subkey_type_count] =
        keys->subscripts[typing->subkey_type_count].expr.type;
  }

  /* Copy the value's type. */
  value = &assign_statement->value_expr;
  typing->value_type = value->type;
  typing->value_as_constexpr_type =
      (typing->value_type == ValueExprType_kConst)
          ? value->variant.as_constexpr.type
          : ConstExprType_kUnspecified;

  return typing;

error:
  return NULL;
}

static int Typing_ResolveLineDiffFromAssignStatement(
    struct Typing* typing, const struct AssignStatement* assign_statement) {
  size_t i;

  const struct KeyExpr* right_key;
  const struct ValueExpr* right_value;
  const struct ConstExpr* right_value_as_constexpr;

  /* Cannot resolve if subkey count is different. */
  right_key = &assign_statement->key_expr;
  if (typing->subkey_type_count != right_key->subscripts_count) {
    return 0;
  }

  /* Resolve subkey type differences. */
  for (i = 0; i < typing->subkey_type_count; ++i) {
    struct ConstExpr* subkey;

    subkey = &right_key->subscripts[i].expr;
    if (typing->subkey_types[i] != subkey->type) {
      typing->subkey_types[i] = ConstExprType_kString;
    }
  }

  /* Resolve value type differences. */
  right_value = &assign_statement->value_expr;
  if (typing->value_type != right_value->type) {
    typing->value_type = ValueExprType_kConst;
    typing->value_as_constexpr_type = ConstExprType_kString;
    return 1;
  }

  right_value_as_constexpr = &right_value->variant.as_constexpr;
  if (typing->value_as_constexpr_type != right_value_as_constexpr->type) {
    typing->value_as_constexpr_type = ConstExprType_kString;
  }

  return 1;
}

/**
 * External
 */

struct Typing* Typing_Init(
    struct Typing* typing, const struct ParserLine* line) {
  switch (line->type) {
    case ParserLineType_kAssignStatement: {
      return Typing_InitFromAssignStatement(typing, &line->variant.assign_statement);
    }

    default: {
      assert(0 && "This should not happen.");
      return NULL;
    }
  }
}

void Typing_Deinit(struct Typing* typing) {
  typing->value_as_constexpr_type = ConstExprType_kUnspecified;
  typing->value_type = ValueExprType_kUnspecified;

  typing->subkey_type_count = 0;
  free(typing->subkey_types);
  typing->subkey_types = NULL;

  typing->key_name = NULL;
}

int Typing_CompareKeyName(
    const struct Typing* left, const struct Typing* right) {
  return ConstExpr_CompareExprAsString(left->key_name, right->key_name);
}

int Typing_Equal(const struct Typing* left, const struct Typing* right) {
  size_t i;

  if (!ConstExpr_Equal(left->key_name, right->key_name)) {
    return 0;
  }

  if (left->subkey_type_count != right->subkey_type_count) {
    return 0;
  }

  for (i = 0; i < left->subkey_type_count; ++i) {
    if (left->subkey_types[i] != right->subkey_types[i]) {
      return 0;
    }
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

  if (left->subkey_type_count != right->subkey_type_count) {
    return 0;
  }

  /* String is the only type that is compatible with all other types. */
  for (i = 0; i < left->subkey_type_count; ++i) {
    if (left->subkey_types[i] != right->subkey_types[i]) {
      left->subkey_types[i] = ConstExprType_kString;
      right->subkey_types[i] = ConstExprType_kString;
    }
  }

  return 1;
}

int Typing_ResolveLineDiff(
    struct Typing* typing, const struct ParserLine* line) {
  switch (line->type) {
    case ParserLineType_kAssignStatement: {
      return Typing_ResolveLineDiffFromAssignStatement(
          typing, &line->variant.assign_statement);
    }

    default: {
      assert(0 && "This should not happen.");
      return 0;
    }
  }
}
