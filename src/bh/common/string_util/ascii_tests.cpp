/**
 * SlashDiablo Maphack
 * Copyright (C) 2012-2022  SlashDiablo Community
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

/**
 * BH
 * Copyright (C) 2011  McGod
 *
 * All rights reserved.
 */

#include "bh/common/string_util/ascii.hpp"

#include <string>
#include <string_view>

namespace bh::common::string_util::ascii {

static_assert(ToLowerChar('\0') == '\0');
static_assert(ToLowerChar(' ') == ' ');
static_assert(ToLowerChar('\t') == '\t');
static_assert(ToLowerChar('a') == 'a');
static_assert(ToLowerChar('z') == 'z');

static_assert(ToLowerChar('A') == 'a');
static_assert(ToLowerChar('B') == 'b');
static_assert(ToLowerChar('C') == 'c');
static_assert(ToLowerChar('D') == 'd');
static_assert(ToLowerChar('E') == 'e');
static_assert(ToLowerChar('F') == 'f');
static_assert(ToLowerChar('G') == 'g');
static_assert(ToLowerChar('H') == 'h');
static_assert(ToLowerChar('I') == 'i');
static_assert(ToLowerChar('J') == 'j');
static_assert(ToLowerChar('K') == 'k');
static_assert(ToLowerChar('L') == 'l');
static_assert(ToLowerChar('M') == 'm');
static_assert(ToLowerChar('N') == 'n');
static_assert(ToLowerChar('O') == 'o');
static_assert(ToLowerChar('P') == 'p');
static_assert(ToLowerChar('Q') == 'q');
static_assert(ToLowerChar('R') == 'r');
static_assert(ToLowerChar('S') == 's');
static_assert(ToLowerChar('T') == 't');
static_assert(ToLowerChar('U') == 'u');
static_assert(ToLowerChar('V') == 'v');
static_assert(ToLowerChar('W') == 'w');
static_assert(ToLowerChar('X') == 'x');
static_assert(ToLowerChar('Y') == 'y');
static_assert(ToLowerChar('Z') == 'z');

static_assert(ToUpperChar('\0') == '\0');
static_assert(ToUpperChar(' ') == ' ');
static_assert(ToUpperChar('\t') == '\t');
static_assert(ToUpperChar('A') == 'A');
static_assert(ToUpperChar('Z') == 'Z');

static_assert(ToUpperChar('a') == 'A');
static_assert(ToUpperChar('b') == 'B');
static_assert(ToUpperChar('c') == 'C');
static_assert(ToUpperChar('d') == 'D');
static_assert(ToUpperChar('e') == 'E');
static_assert(ToUpperChar('f') == 'F');
static_assert(ToUpperChar('g') == 'G');
static_assert(ToUpperChar('h') == 'H');
static_assert(ToUpperChar('i') == 'I');
static_assert(ToUpperChar('j') == 'J');
static_assert(ToUpperChar('k') == 'K');
static_assert(ToUpperChar('l') == 'L');
static_assert(ToUpperChar('m') == 'M');
static_assert(ToUpperChar('n') == 'N');
static_assert(ToUpperChar('o') == 'O');
static_assert(ToUpperChar('p') == 'P');
static_assert(ToUpperChar('q') == 'Q');
static_assert(ToUpperChar('r') == 'R');
static_assert(ToUpperChar('s') == 'S');
static_assert(ToUpperChar('t') == 'T');
static_assert(ToUpperChar('u') == 'U');
static_assert(ToUpperChar('v') == 'V');
static_assert(ToUpperChar('w') == 'W');
static_assert(ToUpperChar('x') == 'X');
static_assert(ToUpperChar('y') == 'Y');
static_assert(ToUpperChar('z') == 'Z');

static_assert(ToLowerChar(L'A') == L'a');
static_assert(ToLowerChar(L'B') == L'b');
static_assert(ToLowerChar(L'C') == L'c');
static_assert(ToUpperChar(L'x') == L'X');
static_assert(ToUpperChar(L'y') == L'Y');
static_assert(ToUpperChar(L'z') == L'Z');

static_assert(ToLowerChar(u8'A') == u8'a');
static_assert(ToLowerChar(u8'B') == u8'b');
static_assert(ToLowerChar(u8'C') == u8'c');
static_assert(ToUpperChar(u8'x') == u8'X');
static_assert(ToUpperChar(u8'y') == u8'Y');
static_assert(ToUpperChar(u8'z') == u8'Z');

static_assert(ToLowerChar(u'A') == u'a');
static_assert(ToLowerChar(u'B') == u'b');
static_assert(ToLowerChar(u'C') == u'c');
static_assert(ToUpperChar(u'x') == u'X');
static_assert(ToUpperChar(u'y') == u'Y');
static_assert(ToUpperChar(u'z') == u'Z');

static_assert(ToLowerChar(U'A') == U'a');
static_assert(ToLowerChar(U'B') == U'b');
static_assert(ToLowerChar(U'C') == U'c');
static_assert(ToUpperChar(U'x') == U'X');
static_assert(ToUpperChar(U'y') == U'Y');
static_assert(ToUpperChar(U'z') == U'Z');

static_assert(
    ToLower("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == "abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToLower("abcdefghijklmnopqrstuvwxyz") == "abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToUpper("abcdefghijklmnopqrstuvwxyz") == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
static_assert(
    ToUpper("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

static_assert(
    ToLower(std::string_view("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
        == "abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToLower(std::string_view("abcdefghijklmnopqrstuvwxyz"))
        == "abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToUpper(std::string_view("abcdefghijklmnopqrstuvwxyz"))
        == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
static_assert(
    ToUpper(std::string_view("ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
        == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

static_assert(
    ToLower(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ") == L"abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToLower(L"abcdefghijklmnopqrstuvwxyz") == L"abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToUpper(L"abcdefghijklmnopqrstuvwxyz") == L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
static_assert(
    ToUpper(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ") == L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");

static_assert(
    ToLower(u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ")
        == u8"abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToLower(u8"abcdefghijklmnopqrstuvwxyz")
        == u8"abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToUpper(u8"abcdefghijklmnopqrstuvwxyz")
        == u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
static_assert(
    ToUpper(u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ")
        == u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ");

static_assert(
    ToLower(u"ABCDEFGHIJKLMNOPQRSTUVWXYZ")
        == u"abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToLower(u"abcdefghijklmnopqrstuvwxyz")
        == u"abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToUpper(u"abcdefghijklmnopqrstuvwxyz")
        == u"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
static_assert(
    ToUpper(u"ABCDEFGHIJKLMNOPQRSTUVWXYZ")
        == u"ABCDEFGHIJKLMNOPQRSTUVWXYZ");

static_assert(
    ToLower(U"ABCDEFGHIJKLMNOPQRSTUVWXYZ")
        == U"abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToLower(U"abcdefghijklmnopqrstuvwxyz")
        == U"abcdefghijklmnopqrstuvwxyz");
static_assert(
    ToUpper(U"abcdefghijklmnopqrstuvwxyz")
        == U"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
static_assert(
    ToUpper(U"ABCDEFGHIJKLMNOPQRSTUVWXYZ")
        == U"ABCDEFGHIJKLMNOPQRSTUVWXYZ");

}  // namespace bh::common::string_util::ascii
