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

#include "bh/d2/dll/name.hpp"

#include <string_view>

#include "bh/common/logging/logger.hpp"
#include "bh/d2/dll/dll.hpp"
#include "bh/global/file_logger.hpp"

namespace bh::d2::dll {
namespace {

using ::bh::common::logging::Logger;
using ::bh::global::GetFileLogger;

static Logger& GetLogger() {
  static Logger& logger = GetFileLogger(__FILEW__);
  return logger;
}

}  // namespace

std::string_view GetName(Dll dll) {
  switch (dll) {
    case Dll::kBnClient: {
      static constexpr std::string_view kName = "BnClient.dll";
      return kName;
    }

    case Dll::kD2Client: {
      static constexpr std::string_view kName = "D2Client.dll";
      return kName;
    }

    case Dll::kD2Cmp: {
      static constexpr std::string_view kName = "D2Cmp.dll";
      return kName;
    }

    case Dll::kD2Common: {
      static constexpr std::string_view kName = "D2Common.dll";
      return kName;
    }

    case Dll::kD2DDraw: {
      static constexpr std::string_view kName = "D2DDraw.dll";
      return kName;
    }

    case Dll::kD2Direct3d: {
      static constexpr std::string_view kName = "D2Direct3d.dll";
      return kName;
    }

    case Dll::kD2Game: {
      static constexpr std::string_view kName = "D2Game.dll";
      return kName;
    }

    case Dll::kD2Gdi: {
      static constexpr std::string_view kName = "D2Gdi.dll";
      return kName;
    }

    case Dll::kD2Gfx: {
      static constexpr std::string_view kName = "D2Gfx.dll";
      return kName;
    }

    case Dll::kD2Glide: {
      static constexpr std::string_view kName = "D2Glide.dll";
      return kName;
    }

    case Dll::kD2Lang: {
      static constexpr std::string_view kName = "D2Lang.dll";
      return kName;
    }

    case Dll::kD2Launch: {
      static constexpr std::string_view kName = "D2Launch.dll";
      return kName;
    }

    case Dll::kD2McpClient: {
      static constexpr std::string_view kName = "D2McpClient.dll";
      return kName;
    }

    case Dll::kD2Multi: {
      static constexpr std::string_view kName = "D2Multi.dll";
      return kName;
    }

    case Dll::kD2Net: {
      static constexpr std::string_view kName = "D2Net.dll";
      return kName;
    }

    case Dll::kD2Sound: {
      static constexpr std::string_view kName = "D2Sound.dll";
      return kName;
    }

    case Dll::kD2Win: {
      static constexpr std::string_view kName = "D2Win.dll";
      return kName;
    }

    case Dll::kFog: {
      static constexpr std::string_view kName = "Fog.dll";
      return kName;
    }

    case Dll::kStorm: {
      static constexpr std::string_view kName = "Storm.dll";
      return kName;
    }
  }

  // This should never happen.
  GetLogger().Fatal(
      __LINE__,
      "Unhandled Dll with value {:d}.",
      static_cast<int>(dll));
  return "";
}

}  // namespace bh::d2::dll
