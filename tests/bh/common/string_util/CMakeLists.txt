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

cmake_minimum_required(VERSION 3.20)

project("Tests.BH.Common.StringUtil")

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
add_executable(${PROJECT_NAME}.AsciiTest "ascii_test.c")
target_compile_definitions(${PROJECT_NAME}.AsciiTest PRIVATE NOMINMAX)
target_include_directories(${PROJECT_NAME}.AsciiTest 
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME}.AsciiTest PRIVATE BH.Common)
add_test(NAME ${PROJECT_NAME}.AsciiTest
    COMMAND ${PROJECT_NAME}.AsciiTest)

add_executable(${PROJECT_NAME}.BoolTest "bool_test.c")
target_compile_definitions(${PROJECT_NAME}.BoolTest PRIVATE NOMINMAX)
target_include_directories(${PROJECT_NAME}.BoolTest 
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME}.BoolTest PRIVATE BH.Common)
add_test(NAME ${PROJECT_NAME}.BoolTest
    COMMAND ${PROJECT_NAME}.BoolTest)

add_executable(${PROJECT_NAME}.IntegerTest "integer_test.c")
target_compile_definitions(${PROJECT_NAME}.IntegerTest PRIVATE NOMINMAX)
target_include_directories(${PROJECT_NAME}.IntegerTest 
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME}.IntegerTest PRIVATE BH.Common)
add_test(NAME ${PROJECT_NAME}.IntegerTest
    COMMAND ${PROJECT_NAME}.IntegerTest)

add_executable(${PROJECT_NAME}.MemstringTest "memstring_test.c")
target_compile_definitions(${PROJECT_NAME}.MemstringTest PRIVATE NOMINMAX)
target_include_directories(${PROJECT_NAME}.MemstringTest 
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME}.MemstringTest PRIVATE BH.Common)
add_test(NAME ${PROJECT_NAME}.MemstringTest
    COMMAND ${PROJECT_NAME}.MemstringTest)
