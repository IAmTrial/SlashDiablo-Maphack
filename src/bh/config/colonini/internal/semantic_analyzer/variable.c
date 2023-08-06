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

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "bh/config/colonini/internal/parser/assign_statement.h"
#include "bh/config/colonini/internal/parser/const_expr.h"
#include "bh/config/colonini/internal/parser/key_expr.h"
#include "bh/config/colonini/internal/parser/parser_line.h"

static struct Variable* Variable_InitPartialFromAssignStatement(
    struct Variable* variable,
    const struct AssignStatement* assign_statement) {
  /* Reuse the key name. */
  variable->key_expr = &assign_statement->key_expr;

  return variable;
}

/**
 * External
 */

struct Variable* Variable_Init(
    struct Variable* variable, const struct ParserLine* line) {
  const struct AssignStatement* assign_statement;
  const struct KeyExpr* keys;

  /* Copy the line number. */
  variable->line_number = line->line_number;

  switch (line->type) {
    case ParserLineType_kAssignStatement: {
      return Variable_InitPartialFromAssignStatement(
          variable, &line->variant.assign_statement);
    }

    default: {
      assert(0 && "This should never happen.");
      return NULL;
    }
  }
}

void Variable_Deinit(struct Variable* variable) {
  variable->key_expr = NULL;
  variable->line_number = 0;
}

int Variable_CompareKeysAsStrings(
    const struct Variable* left, const struct Variable* right) {
  return KeyExpr_CompareKeysAsStrings(left->key_expr, right->key_expr);
}

int Variable_EqualKeys(
    const struct Variable* left, const struct Variable* right) {
  return KeyExpr_Equal(left->key_expr, right->key_expr);
}

int Variable_EqualName(
    const struct Variable* left, const struct Variable* right) {
  return ConstExpr_Equal(
      &left->key_expr->constexpr, &right->key_expr->constexpr);
}
