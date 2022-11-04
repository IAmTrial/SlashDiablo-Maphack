#include "Colorhook.h"

#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#include <string>

#include "../../Hook.h"

#include "../../../Common.h"
#include "../../../D2Ptrs.h"
#include "../../Basic/Boxhook/Boxhook.h"
#include "../../Basic/Crosshook/Crosshook.h"
#include "../../Basic/Framehook/Framehook.h"
#include "../../Basic/Texthook/Texthook.h"

namespace Drawing {

Colorhook* Colorhook::current;

/* Basic Hook Initializer
 *		Used for just drawing basics.
 */
Colorhook::Colorhook(
		HookVisibility visibility,
		unsigned int x,
		unsigned int y,
		unsigned int* color,
		std::wstring text)
				: Hook(visibility, x, y),
					text_(std::move(text)) {}

/* Group Hook Initializer
 *		Used in conjuction with other basic hooks to create an advanced hook.
 */
Colorhook::Colorhook(
		HookGroup* group,
		unsigned int x,
		unsigned int y,
		unsigned int* color,
		std::wstring text)
				: Hook(group, x, y),
					text_(std::move(text)) {}

void Colorhook::SetColor(unsigned int newColor) {
	if (newColor < 0 || newColor > 255)
		return;
	Lock();
	*currentColor = newColor;
	Unlock();
}

bool Colorhook::OnLeftClick(bool up, unsigned int x, unsigned int y) {
	if (Colorhook::current == this && x >= 310 && y >= 205 && x <= 490 && y <= 385 && up) {
		SetColor(curColor);
		Colorhook::current = nullptr;
		return true;
	} else if (InRange(x,y) && Colorhook::current == nullptr) {
		if (up)
			Colorhook::current = this;
		return true;
	}
	return false;
}

bool Colorhook::OnRightClick(bool up, unsigned int x, unsigned int y) {
	if (Colorhook::current == this) {
		Colorhook::current = nullptr;
		return true;
	}
	return false;
}

/* GetXSize()
 *	Returns how long the text is.
 */
unsigned int Colorhook::GetXSize() {
	DWORD width, fileNo;
	DWORD oldFont = D2WIN_SetTextSize(0);
	D2WIN_GetTextWidthFileNo(text_.c_str(), &width, &fileNo);
	D2WIN_SetTextSize(oldFont);
	return width; 
}

/* GetYSize()
 *	Returns how tall the text is.
 */
unsigned int Colorhook::GetYSize() {
	return 10;
}

void Colorhook::OnDraw() {
	Lock();
	if (Colorhook::current == this) {
		//Draw the shaded background
		Boxhook::Draw(0, 0, Hook::GetScreenWidth(), Hook::GetScreenHeight(), 0, BTOneHalf);
		//Draw the actual choose color box
		Framehook::Draw(310, 180, 180, 220, 0, BTNormal);
		//Draw title
		Texthook::Draw(360, 186, false, 0, White, L"Choose Color");
		int col = 1, boxX1, boxX2, boxY1, boxY2;
		int mX = (*p_D2CLIENT_MouseX);
		int mY = (*p_D2CLIENT_MouseY);
		for (int n = 1, row = 1; n <= 255; n++, row++) {
			if (row == 16) {
				col++;
				row = 0;
			}
			//Color square begin/end pixel coordinates
			boxX1 = 321 + (row * 10);
			boxX2 = 331 + (row * 10);
			boxY1 = 190 + (col * 10);
			boxY2 = 200 + (col * 10);
			//Set current color based on mouse location
			if (mX >= boxX1 && mY >= boxY1 && mX <= boxX2 && mY <= boxY2)
				curColor = n;
			//Draw each color box
			D2GFX_DrawRectangle(boxX1, boxY1, boxX2, boxY2, n, 5);
		}
		//Draw the +ish symbol showing the currently hovered color
		CHAR szLines[][2] = { 0,-2, 4,-4, 8,-2, 4,0, 8,2, 4,4, 0,2, -4,4, -8,2, -4,0, -8,-2, -4,-4, 0,-2 };
		for (unsigned int x = 0; x < 12; x++)
			D2GFX_DrawLine(457 + szLines[x][0], 380 + szLines[x][1], 457 + szLines[x + 1][0], 380 + szLines[x + 1][1], curColor, -1);
		//Draw instructions
		Texthook::Draw(320, 384, false, 0, White, L"Left Click - Select");
		Texthook::Draw(320, 368, false, 0, White, L"Right Click - Close");
	} else {
		DWORD size = D2WIN_SetTextSize(0);
		D2WIN_DrawText(text_.c_str(), GetX() + 13, GetY() + 10, InRange(*p_D2CLIENT_MouseX, *p_D2CLIENT_MouseY)?7:4, 0);
		D2WIN_SetTextSize(size);
		Crosshook::Draw(GetX(), GetY() + 4, GetColor());
	}
	Unlock();
}

}  // namespace Drawing
