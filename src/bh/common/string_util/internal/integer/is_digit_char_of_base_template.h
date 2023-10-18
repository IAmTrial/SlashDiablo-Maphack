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

/**
 * BH
 * Copyright (C) 2011  McGod
 *
 * All rights reserved.
 */

#if !defined(T_CHAR)
#error Define T_CHAR to specify the templated character type.
#endif  /* !defined(T_CHAR) */

#if !defined(T_FROM_DIGIT_CHAR_FUNC_NAME)
#error Define T_FROM_DIGIT_CHAR_FUNC_NAME to specify the Integer_FromDigitChar function name.
#endif  /* !defined(T_FROM_DIGIT_CHAR_FUNC_NAME) */

#if !defined(T_FUNC_NAME)
#error Define T_FUNC_NAME to specify the function name.
#endif  /* !defined(T_FUNC_NAME) */

int T_FUNC_NAME(T_CHAR ch, int base) {
  int value;

  return (T_FROM_DIGIT_CHAR_FUNC_NAME(&value, ch, base) != NULL);
}

#undef T_FUNC_NAME

#undef T_FROM_DIGIT_CHAR_FUNC_NAME

#undef T_CHAR
