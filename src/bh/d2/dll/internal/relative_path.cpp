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

#include "bh/d2/dll/internal/relative_path.hpp"

#include <string_view>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/dll/dll.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::dll::internal {
namespace {

using ::bh::common::logging::Logger;
using ::bh::d2::dll::Dll;
using ::bh::global::GetFileLogger;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

}  // namespace

std::wstring_view GetRelativePath(Dll dll) {
  switch (dll) {
    case Dll::kBnClient: {
      static constexpr std::wstring_view kPath = L"BnClient.dll";
      return kPath;
    }

    case Dll::kD2Client: {
      static constexpr std::wstring_view kPath = L"D2Client.dll";
      return kPath;
    }

    case Dll::kD2Cmp: {
      static constexpr std::wstring_view kPath = L"D2Cmp.dll";
      return kPath;
    }

    case Dll::kD2Common: {
      static constexpr std::wstring_view kPath = L"D2Common.dll";
      return kPath;
    }

    case Dll::kD2DDraw: {
      static constexpr std::wstring_view kPath = L"D2DDraw.dll";
      return kPath;
    }

    case Dll::kD2Direct3d: {
      static constexpr std::wstring_view kPath = L"D2Direct3d.dll";
      return kPath;
    }

    case Dll::kD2Game: {
      static constexpr std::wstring_view kPath = L"D2Game.dll";
      return kPath;
    }

    case Dll::kD2Gdi: {
      static constexpr std::wstring_view kPath = L"D2Gdi.dll";
      return kPath;
    }

    case Dll::kD2Gfx: {
      static constexpr std::wstring_view kPath = L"D2Gfx.dll";
      return kPath;
    }

    case Dll::kD2Glide: {
      static constexpr std::wstring_view kPath = L"D2Glide.dll";
      return kPath;
    }

    case Dll::kD2Lang: {
      static constexpr std::wstring_view kPath = L"D2Lang.dll";
      return kPath;
    }

    case Dll::kD2Launch: {
      static constexpr std::wstring_view kPath = L"D2Launch.dll";
      return kPath;
    }

    case Dll::kD2McpClient: {
      static constexpr std::wstring_view kPath = L"D2McpClient.dll";
      return kPath;
    }

    case Dll::kD2Multi: {
      static constexpr std::wstring_view kPath = L"D2Multi.dll";
      return kPath;
    }

    case Dll::kD2Net: {
      static constexpr std::wstring_view kPath = L"D2Net.dll";
      return kPath;
    }

    case Dll::kD2Sound: {
      static constexpr std::wstring_view kPath = L"D2Sound.dll";
      return kPath;
    }

    case Dll::kD2Win: {
      static constexpr std::wstring_view kPath = L"D2Win.dll";
      return kPath;
    }

    case Dll::kFog: {
      static constexpr std::wstring_view kPath = L"Fog.dll";
      return kPath;
    }

    case Dll::kStorm: {
      static constexpr std::wstring_view kPath = L"Storm.dll";
      return kPath;
    }
  }

  // This should never happen.
  GetLogger().Fatal(
      __LINE__,
      "Unhandled Dll with value {:d}.",
      static_cast<int>(dll));
  return L"";
}

}  // namespace bh::d2::dll::internal
