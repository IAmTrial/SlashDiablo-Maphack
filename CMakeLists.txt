# SlashDiablo Maphack
# Copyright (C) 2012-2022  SlashDiablo Community
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

cmake_minimum_required(VERSION 3.24)

# Name of the project, also is the name of the file
project(BH)

# Define requirements for C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define requirements for C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Remove MinGW compiled binary "lib" prefix
if (MINGW)
    set(CMAKE_IMPORT_LIBRARY_PREFIX "")
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
endif (MINGW)

# Define DLL target
add_library(${PROJECT_NAME} SHARED)

set(SHA CACHE STRING "Git commit SHA hash.")
mark_as_advanced(SHA)

if (NOT ${SHA} STREQUAL "")
    string(SUBSTRING "${SHA}" 0 7 SHORT_SHA)
    message(NOTICE "Git SHA: $CACHE{SHA}")
    message(NOTICE "Git Short SHA: ${SHORT_SHA}")
    add_compile_definitions(SHA="${SHORT_SHA}")
endif (NOT ${SHA} STREQUAL "")

add_subdirectory(BH)
add_subdirectory(src)

target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX)
target_compile_options(${PROJECT_NAME} PRIVATE /permissive /Zc:preprocessor)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/cpp-lru-cache/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} shlwapi)
