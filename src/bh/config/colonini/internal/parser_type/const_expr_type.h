
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

#ifndef BH_CONFIG_COLONINI_INTERNAL_PARSER_TYPE_CONST_EXPR_TYPE_H_
#define BH_CONFIG_COLONINI_INTERNAL_PARSER_TYPE_CONST_EXPR_TYPE_H_

#include <stddef.h>

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

const char* ConstExprType_GetDisplayName(
    enum ConstExprType type, size_t* length);

enum ConstExprType ConstExprType_MatchString(
    const char* str, size_t length);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* BH_CONFIG_COLONINI_INTERNAL_PARSER_TYPE_CONST_EXPR_TYPE_H_ */
