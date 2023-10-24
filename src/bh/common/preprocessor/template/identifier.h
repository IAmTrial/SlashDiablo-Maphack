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

#ifndef BH_COMMON_PREPROCESSOR_TEMPLATE_IDENTIFIER_H_
#define BH_COMMON_PREPROCESSOR_TEMPLATE_IDENTIFIER_H_

#include "bh/common/preprocessor/concat.h"

#define TEMPLATE_IDENTIFIER_1(base, t1) \
  PREPROCESSOR_CONCAT( \
      PREPROCESSOR_CONCAT(PREPROCESSOR_CONCAT(base, _), t1), _)

#define TEMPLATE_IDENTIFIER_2(base, t1, t2) \
  PREPROCESSOR_CONCAT( \
      PREPROCESSOR_CONCAT( \
          TEMPLATE_IDENTIFIER_1(base, t1), t2), _)

#define TEMPLATE_IDENTIFIER_3(base, t1, t2, t3) \
  PREPROCESSOR_CONCAT( \
      PREPROCESSOR_CONCAT( \
          TEMPLATE_IDENTIFIER_2(base, t1, t2), t3), _)

#define TEMPLATE_IDENTIFIER_4(base, t1, t2, t3, t4) \
  PREPROCESSOR_CONCAT( \
      PREPROCESSOR_CONCAT( \
          TEMPLATE_IDENTIFIER_3(base, t1, t2, t3), t4), _)

#define TEMPLATE_IDENTIFIER_5(base, t1, t2, t3, t4, t5) \
  PREPROCESSOR_CONCAT( \
      PREPROCESSOR_CONCAT( \
          TEMPLATE_IDENTIFIER_4(base, t1, t2, t3, t4), t5), _)

#define TEMPLATE_IDENTIFIER_6(base, t1, t2, t3, t4, t5, t6) \
  PREPROCESSOR_CONCAT( \
      PREPROCESSOR_CONCAT( \
          TEMPLATE_IDENTIFIER_5(base, t1, t2, t3, t4, t5), t6), _)

#define TEMPLATE_IDENTIFIER_7(base, t1, t2, t3, t4, t5, t6, t7) \
  PREPROCESSOR_CONCAT( \
      PREPROCESSOR_CONCAT( \
          TEMPLATE_IDENTIFIER_6( \
              base, t1, t2, t3, t4, t5, t6), t7), _)

#define TEMPLATE_IDENTIFIER_8(base, t1, t2, t3, t4, t5, t6, t7, t8) \
  PREPROCESSOR_CONCAT( \
      PREPROCESSOR_CONCAT( \
          TEMPLATE_IDENTIFIER_7( \
              base, t1, t2, t3, t4, t5, t6, t7), t8), _)

#define TEMPLATE_IDENTIFIER_9( \
    base, t1, t2, t3, t4, t5, t6, t7, t8, t9) \
  PREPROCESSOR_CONCAT( \
      PREPROCESSOR_CONCAT( \
          TEMPLATE_IDENTIFIER_8( \
              base, t1, t2, t3, t4, t5, t6, t7, t8), t9), _)

#endif  /* BH_COMMON_PREPROCESSOR_TEMPLATE_IDENTIFIER_H_ */
