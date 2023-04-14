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

#ifndef BH_CONFIG_COLONINI_INTERNAL_PARSER_H_
#define BH_CONFIG_COLONINI_INTERNAL_PARSER_H_

#include <stddef.h>

#include "bh/config/colonini/internal/lexer.h"
#include "bh/config/colonini/type.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

enum ConstExprType {
  ConstExprType_kUnspecified,
  ConstExprType_kInvalid,

  ConstExprType_kString,
  ConstExprType_kSignedInt,
  ConstExprType_kUnsignedInt,
  ConstExprType_kBoolean
};

struct ConstExpr {
  enum ConstExprType type;
  char* expr;
  size_t length;

  const struct LexerString* begin_src;
  const struct LexerString* end_src;
};

struct ToggleExpr {
  struct ConstExpr enabled_expr;
  struct ConstExpr input_expr;
};

struct Subscript {
  struct ConstExpr expr;
};

struct KeyExpr {
  struct ConstExpr constexpr;
  struct Subscript* subscripts;
  size_t subscripts_count;
};

enum ValueExprType {
  ValueExprType_kUnspecified,

  ValueExprType_kConst,
  ValueExprType_kToggle,
};

struct ValueExpr {
  enum ValueExprType type;
  union {
    struct ConstExpr as_constexpr;
    struct ToggleExpr as_toggleexpr;
  } variant;
};

struct AssignStatement {
  struct KeyExpr key_expr;
  struct ValueExpr value_expr;
};

enum ParserLineType {
  ParserLineType_kUnspecified,
  ParserLineType_kInvalid,

  ParserLineType_kNoOp,
  ParserLineType_kAssignStatement
};

struct ParserLine {
  int line_number;

  enum ParserLineType type;
  union {
    struct AssignStatement assign_statement;
  } variant;
};

/**
 * Parses a LexerLine into a ParserLine, resolving the key, subkeys,
 * value type, and value. If the line is invalid, NULL is returned.
 */
struct ParserLine* ParserLine_ParseLine(
    struct ParserLine* parser_line,
    const struct LexerLine* lexer_line,
    size_t* error_column);

/**
 * Deinitializes a ParserLine, freeing up resources that were allocated.
 */
void ParserLine_Deinit(struct ParserLine* parser_line);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_PARSER_H_ */
