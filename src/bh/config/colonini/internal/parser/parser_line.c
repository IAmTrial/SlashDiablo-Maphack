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

#include "bh/config/colonini/internal/parser/parser_line.h"

#include <assert.h>
#include <stddef.h>

#include "bh/config/colonini/internal/lexer.h"
#include "bh/config/colonini/internal/parser/assign_statement.h"

/*
 * Parse rules:
 * S -> K : V
 *
 * K -> sP
 * P -> AP
 * A -> [d]
 * A -> [s]
 * P -> e
 * V -> R
 * R -> bT
 * T -> , s
 * T -> e
 * R -> s
 * V -> d
 * V -> h
 *
 * Terminals (regex):
 * e(mpty) ->
 * b(oolean) -> (true)|(false)
 * d(ecimal) -> (0|([1-9][0-9]{0,9}))
 * h(exadecimal) -> 0(x|X)[0-9A-Fa-f]{1,7}
 * s(tring) -> [^\t\n\v\f\r ][^\[\]:]*[^\[\]:\t\n\v\f\r ]
 */

/**
 * External
 */

struct ParserLine* ParserLine_ParseLine(
    struct ParserLine* parser_line,
    const struct LexerLine* lexer_line,
    size_t* error_column) {
  struct AssignStatement* parse_assign_statement_result;

  parser_line->line_number = lexer_line->line_number;

  if (lexer_line->tokens_count == 0) {
    parser_line->type = ParserLineType_kNoOp;
    return parser_line;
  }

  parse_assign_statement_result =
      AssignStatement_Parse(
          &parser_line->variant.assign_statement,
          lexer_line->first_token,
          &lexer_line->last_token[1],
          error_column);
  if (parse_assign_statement_result == NULL) {
    parser_line->type = ParserLineType_kInvalid;
    return NULL;
  }

  parser_line->type = ParserLineType_kAssignStatement;

  return parser_line;
}

void ParserLine_Deinit(struct ParserLine* parser_line) {
  switch (parser_line->type) {
    case ParserLineType_kNoOp:
    case ParserLineType_kInvalid: {
      break;
    }

    case ParserLineType_kAssignStatement: {
      AssignStatement_Deinit(&parser_line->variant.assign_statement);
      break;
    }

    default: {
      assert(0 && "This should never happen.");
      return;
    }
  }
  parser_line->line_number = 0;
}
