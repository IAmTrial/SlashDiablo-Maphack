/**
 * SlashDiablo Maphack
 * Copyright (C) SlashDiablo Community
 *
 * BH
 * Copyright 2011 (C) McGod
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

#ifndef BH_D2_HELPER_PRINT_TO_CHAT_H_
#define BH_D2_HELPER_PRINT_TO_CHAT_H_

#include <stdarg.h>
#include <wchar.h>
#include <windows.h>

namespace d2helper::chat {

/**
 * Prints a formatted string as a chat message.
 */
int printf(int color, const char* format, ...);

/**
 * Prints a formatted string as a chat message.
 */
int wprintf(int color, const wchar_t* format, ...);

/**
 * Prints a formatted string as a chat message.
 */
int vprintf(int color, const char* format, va_list args);

/**
 * Prints a formatted string as a chat message.
 */
int vwprintf(int color, const wchar_t* format, va_list args);

} // namespace d2helper::chat

#endif // BH_D2_HELPER_PRINT_TO_CHAT_H_
