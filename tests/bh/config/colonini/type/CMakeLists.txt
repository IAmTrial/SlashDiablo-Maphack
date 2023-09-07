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

cmake_minimum_required(VERSION 3.20)

project("Tests.BH.Config.Colonini.Type")

# Define requirements for C
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)

unset(CMAKE_CXX_STANDARD)
unset(CMAKE_CXX_STANDARD_REQUIRED)

# Remove MinGW compiled binary "lib" prefix
if (MINGW)
    set(CMAKE_IMPORT_LIBRARY_PREFIX "")
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
endif (MINGW)

# Define test targets
add_executable(${PROJECT_NAME}.DataTest "data_test.c")
target_compile_definitions(${PROJECT_NAME}.DataTest PRIVATE NOMINMAX)
target_include_directories(${PROJECT_NAME}.DataTest
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME}.DataTest PRIVATE shlwapi BH.Config.Colonini)

add_executable(${PROJECT_NAME}.StringTest "string_test.c")
target_compile_definitions(${PROJECT_NAME}.StringTest PRIVATE NOMINMAX)
target_include_directories(${PROJECT_NAME}.StringTest
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME}.StringTest PRIVATE shlwapi BH.Config.Colonini)

add_executable(${PROJECT_NAME}.ValueTest "value_test.c")
target_compile_definitions(${PROJECT_NAME}.ValueTest PRIVATE NOMINMAX)
target_include_directories(${PROJECT_NAME}.ValueTest
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME}.ValueTest PRIVATE shlwapi BH.Config.Colonini)
