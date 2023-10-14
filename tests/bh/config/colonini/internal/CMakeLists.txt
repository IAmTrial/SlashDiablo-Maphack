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

project(Test.BH.Config.Colonini.Internal)

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

# Copy test data files
file(
    COPY
        "data/empty.cfg"
        "data/one_line.cfg"
        "data/one_line_newline.cfg"
        "data/two_line.cfg"
        "data/two_line_newline.cfg"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/data)

# Define test targets
add_executable(${PROJECT_NAME}.FileReaderTest "file_reader_test.c")
target_compile_definitions(${PROJECT_NAME}.FileReaderTest PRIVATE NOMINMAX)
target_include_directories(${PROJECT_NAME}.FileReaderTest 
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME}.FileReaderTest PRIVATE shlwapi BH.Config.Colonini)

add_executable(${PROJECT_NAME}.LexerTest "lexer_test.c")
target_compile_definitions(${PROJECT_NAME}.LexerTest PRIVATE NOMINMAX)
target_include_directories(${PROJECT_NAME}.LexerTest 
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME}.LexerTest PRIVATE BH.Config.Colonini)
