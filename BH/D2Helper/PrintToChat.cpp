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

#include "PrintToChat.h"

#include <stdarg.h>
#include <wchar.h>
#include <windows.h>

#include <array>
#include <ios>
#include <fstream>
#include <string_view>

#include "../Common/WideEncoding.h"
#include "../D2Ptrs.h"

namespace d2helper::chat {
namespace {

// TODO (Mir Drualga): Actual character length limit might be higher,
// but the limit that is set here is what we know to not cause crashes.
static inline constexpr int kMessageLengthLimit = 152;

static inline constexpr std::wstring_view kLogPath = L"./BH.D2Helper.log";

static std::ofstream& GetLogger() {
  static std::ofstream logger(kLogPath.data(), std::ios_base::app);
  return logger;
}

} // namespace

int printf(int color, const char* format, ...) {
  va_list args;

  va_start(args, format);
  int result = d2helper::chat::vprintf(color, format, args);
  va_end(args);

  return result;
}

int wprintf(int color, const wchar_t* format, ...) {
  va_list args;

  va_start(args, format);
  int result = d2helper::chat::vwprintf(color, format, args);
  va_end(args);

  return result;
}

int vprintf(int color, const char* format, va_list args) {
  std::array<char, kMessageLengthLimit> message;
  int result = _vsnprintf(
      message.data(),
      message.size(),
      format,
      args);

  std::wstring wide_message = common::wide::Utf8ToWide(message.data());
  D2CLIENT_PrintGameString(wide_message.data(), color);

  return result;
}

int vwprintf(int color, const wchar_t* format, va_list args) {
  std::array<wchar_t, kMessageLengthLimit> message;
  int result = _vsnwprintf(
      message.data(),
      message.size(),
      format,
      args);

  D2CLIENT_PrintGameString(message.data(), color);

  return result;
}

} // namespace d2helper::chat
