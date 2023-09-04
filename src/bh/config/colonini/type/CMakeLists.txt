# SlashDiablo Maphack
# Copyright (C) 2012-2022  SlashDiablo Team
#
# This file is part of SlashDiablo Maphack.
#
# SlashDiablo Maphack is free software: you can redistribute it
# and/or modify it under the terms of the GNU Affero General Public
# License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public
# License along with this program.  If not, see
# <http://www.gnu.org/licenses/>.

target_sources(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/data.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/entry.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/key.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/list.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/map.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/string.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/string.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/value.h"
)
