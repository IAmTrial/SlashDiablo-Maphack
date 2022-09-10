/**
*
* Common.cpp
* BH: Copyright 2011 (C) McGod
* SlashDiablo Maphack: Copyright (C) SlashDiablo Community
*
*  This file is part of SlashDiablo Maphack.
*
*  SlashDiablo Maphack is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Affero General Public License as published
*  by the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Affero General Public License for more details.
*
*  You should have received a copy of the GNU Affero General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* This file incorporates work covered by the following copyright and
* permission notice:
*
*   ==========================================================
*   D2Ex2
*   https://github.com/lolet/D2Ex2
*   ==========================================================
*   Copyright (c) 2011-2014 Bartosz Jankowski
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*   ==========================================================
*
*/

#include "Common.h"

#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <windows.h>

#include <algorithm>
#include <ios>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "BH.h"
#include "Constants.h"
#include "D2Ptrs.h"

void Tokenize(const std::string& str,
                      std::vector<std::string>& tokens,
                      const std::string& delimiters = " ")
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

wchar_t* AnsiToUnicode(const char* str)
{
	wchar_t* buf = NULL;
	int len = MultiByteToWideChar(CODE_PAGE, 0, str, -1, buf, 0);
	buf = new wchar_t[len];
	MultiByteToWideChar(CODE_PAGE, 0, str, -1, buf, len);
	return buf;
}

char* UnicodeToAnsi(const wchar_t* str)
{
	char* buf = NULL;
	int len = WideCharToMultiByte(CODE_PAGE, 0, str, -1, buf, 0, "?", NULL);
	buf = new char[len];
	WideCharToMultiByte(CODE_PAGE, 0, str, -1, buf, len, "?", NULL);
	return buf;
}

std::wstring GetColorCode(int ColNo)
{
	wchar_t* pCol = D2LANG_GetLocaleText(3994);
	std::wostringstream Result;
	Result << pCol << (wchar_t) (ColNo + '0');
	return Result.str();
}

// This function prints at most 151 characters (152 including null)
// TODO: Fix this so this limitation
void PrintText(DWORD Color, char *szText, ...) {
	char szBuffer[152] = {0};
	va_list Args;
	va_start(Args, szText);
	vsnprintf_s(szBuffer, 152, _TRUNCATE, szText, Args);
	va_end(Args); 
	wchar_t Buffer[0x130];
	MultiByteToWideChar(CODE_PAGE, 1, szBuffer, 152, Buffer, 304);
	D2CLIENT_PrintGameString(Buffer, Color);	
}

ULONGLONG BHGetTickCount(void) {
    static bool first = true;
    static ULONGLONG (*pGetTickCount64)(void);

    if (first) {
        HMODULE hlib = LoadLibraryA("KERNEL32.DLL");
        pGetTickCount64 = (ULONGLONG(*)())GetProcAddress(hlib, "GetTickCount64");
        first = false;
    }
    if (pGetTickCount64) {
        return pGetTickCount64();
	}
    return (ULONGLONG)GetTickCount();
}

// See: http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
std::string string_format(const std::string fmt_str, ...) {
	int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::string str;
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1) {
		formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return std::string(formatted.get());
}


VOID *memcpy2(void *dest, const void *src, size_t count)
{
	return (char *)memcpy(dest, src, count) + count;
}

HANDLE OpenFileRead(char *filename)
{
	return CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}
BYTE *AllocReadFile(char *filename)
{
	HANDLE hFile = OpenFileRead(filename);
	int filesize = GetFileSize(hFile, 0);
	if (filesize <= 0) return 0;
	BYTE *buf = new BYTE[filesize];
	ReadFile(hFile, buf, filesize);
	CloseHandle(hFile);
	return buf;
}
DWORD ReadFile(HANDLE hFile, void *buf, DWORD len)
//NOTE :- validates len bytes of buf
{
	DWORD numdone = 0;
	ReadFile(hFile, buf, len, &numdone, NULL);
	return numdone;
}

char *GetMyFileNameStrrchr(char *dest, char ch)
{
	GetModuleFileName(BH::instance, dest, MAX_PATH);
	return strrchr(dest, ch) + 1;
}

#define PI 3.1415926535

long CalculateDistance(const POINT& pt1, const POINT& pt2)
{
	return CalculateDistance(pt1.x, pt1.y, pt2.x, pt2.y);
}

long CalculateAngle(const POINT& pt1, const POINT& pt2)
{
	return CalculateAngle(pt1.x, pt1.y, pt2.x, pt2.y);
}

long CalculateDistance(long x1, long y1, long x2, long y2)
{
	return (long)::sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

BOOL PtInCircle(const POINT& pt, const POINT& ptOrigin, int nRadius)
{
	return CalculateDistance(pt, ptOrigin) < ::abs(nRadius);
}

void NormalizeAngle(int& rAngle)
{
	if (::abs(rAngle) >= 360)
		rAngle %= 360;

	if (rAngle < 0)
		rAngle += 360;
}

void NormalizeRect(RECT& rRect)
{
	NormalizeRect(&rRect);
}

void NormalizeRect(LPRECT lpRect)
{
	if (lpRect == NULL)
		return;

	long temp;
	if (lpRect->left > lpRect->right)
	{
		temp = lpRect->left;
		lpRect->left = lpRect->right;
		lpRect->right = temp;
	}

	if (lpRect->top > lpRect->bottom)
	{
		temp = lpRect->top;
		lpRect->top = lpRect->bottom;
		lpRect->bottom = temp;
	}
}

long CalculateAngle(long x1, long y1, long x2, long y2)
{
	// mathematic stuff, now thanks God I haven't forgot all of them...
	if (x1 == x2 && y1 == y2)
		return 0;

	double fAngle = 0.0;

	if (x1 == x2)
	{
		// vertical special case
		fAngle = y2 > y1 ? 270.0 : 90.0;
	}
	else if (y1 == y2)
	{
		// horizontal special case
		fAngle = x2 > x1 ? 0.0 : 180.0;
	}
	else
	{
		// common case
		fAngle = ::atan(((double)y2 - (double)y1) / ((double)x2 - (double)x1)) * 180.0 / PI;

		// determine the phases (1-4)
		// Phases allocation figure:
		/*
		y
		|
		P2     |    P1
		|
		-----------+----------->x
		|
		P3     |    P4
		|
		*/
		int nPhase = 0;
		if (y2 < y1)
			nPhase = x2 > x1 ? 1 : 2;
		else
			nPhase = x2 > x1 ? 4 : 3;

		// adjust the angle according to phases
		switch (nPhase)
		{
		case 1:
			fAngle = -fAngle;
			break;

		case 2:
			fAngle = 180.0 - fAngle;
			break;

		case 3:
			fAngle = 180.0 - fAngle;
			break;

		case 4:
			fAngle = 360.0 - fAngle;
			break;

		default:
			fAngle = 0.0;
			break;
		}
	}

	return (long)fAngle;
}

POINT CalculatePointOnTrack(const POINT& ptOrigin, int nRadius, int nAngle)
{
	if (nRadius == 0)
		return ptOrigin;

	NormalizeAngle(nAngle);

	POINT pt;
	pt.x = long(double(ptOrigin.x) + ::cos((double)nAngle * PI / 180.0) * (double)nRadius);
	pt.y = long(double(ptOrigin.y) - ::sin((double)nAngle * PI / 180.0) * (double)nRadius);
	return pt;
}

POINT CalculateRandomPosition(const POINT& ptOrigin, int nRadiusMin, int nRadiusMax, int nAngleMin/*=0*/, int nAngleMax/*=360*/)
{
	// Data validation
	nRadiusMin = std::max(0, nRadiusMin);
	nRadiusMax = std::max(0, nRadiusMax);

	NormalizeAngle(nAngleMin);
	NormalizeAngle(nAngleMax);

	const int R1 = std::min(nRadiusMin, nRadiusMax);
	const int R2 = std::max(nRadiusMin, nRadiusMax);
	const int A1 = std::min(nAngleMin, nAngleMax);
	const int A2 = std::max(nAngleMin, nAngleMax);

	const int R = (R1 == R2) ? R1 : (R1 + (::rand() % (R2 - R1))); // Final radius
	const int A = (A1 == A2) ? A1 : (A1 + (::rand() % (A2 - A1))); // Final angle

	return CalculatePointOnTrack(ptOrigin, R, A);
}

char *commaprint(unsigned long n)
{
	int comma = '\0';
	char retbuf[30];
	char *p = &retbuf[sizeof(retbuf) - 1];
	int i = 0;

	if (comma == '\0') {
		struct lconv *lcp = localeconv();
		if (lcp != NULL) {
			if (lcp->thousands_sep != NULL &&
				*lcp->thousands_sep != '\0')
				comma = *lcp->thousands_sep;
			else	comma = ',';
		}
	}

	*p = '\0';

	do {
		if (i % 3 == 0 && i != 0)
			*--p = comma;
		*--p = '0' + n % 10;
		n /= 10;
		i++;
	} while (n != 0);

	return p;
}
