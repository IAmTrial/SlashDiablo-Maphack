# SlashDiablo Maphack
# Copyright (C) 2012-2023  SlashDiablo Team
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
        "${CMAKE_CURRENT_SOURCE_DIR}/memchr.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/memchr.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/memcmp.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/memcmp.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/memcpy.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/memcpy.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/memcrspn.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/memcrspn.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/memcrspn_template.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/memcspn.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/memcspn.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/memcspn_template.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/memrspn.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/memrspn.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/memrspn_template.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/memspn.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/memspn.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/memspn_template.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/memstr.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/memstr.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/memstr_template.h"
)
