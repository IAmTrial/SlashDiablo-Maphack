#include "Inputhook.h"

#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#include <array>
#include <string>

#include "../../Basic/Framehook/Framehook.h"
#include "../../Basic/Texthook/Texthook.h"
#include "../../../Common.h"
#include "../../../D2Ptrs.h"
#include "../../Hook.h"

namespace Drawing {

Inputhook::Inputhook(
		HookVisibility visibility,
		unsigned int x,
		unsigned int y,
		unsigned int xSize,
		std::wstring text)
				: Hook(visibility, x, y),
					text_(std::move(text)) {
	SetXSize(xSize);
	SetFont(0);
	SetActive(false);
	SetCursorState(true);
	ResetCursorTick();
	ResetSelection();
	textPos = 0;
	SetCursorPosition(text.length());
}

Inputhook::Inputhook(
		HookGroup* group,
		unsigned int x,
		unsigned int y,
		unsigned int xSize,
		std::wstring text)
				: Hook(group, x, y),
					text_(std::move(text)) {
	SetXSize(xSize);
	SetFont(0);
	SetActive(false);
	SetCursorState(true);
	ResetCursorTick();
	ResetSelection();
	textPos = 0;
	SetCursorPosition(text_.length());
}

 void Inputhook::SetFont(unsigned int newFont) {
	if (newFont >=  0 && newFont < 16) {
		Lock();
		font = newFont;
		Unlock();
	}
 }

 void Inputhook::SetXSize(unsigned int newXSize) {
	// if (newXSize > 0 && (Hook::GetScreenWidth() - GetX()) < newXSize) {
		 Lock();
		 xSize = newXSize;
		 Unlock();
	// }
 }

 void Inputhook::CursorTick() {
	  if (cursorTick % 30 == 0) { 
		  ResetCursorTick(); 
		  ToggleCursor(); 
	  }
	  cursorTick++;
 }

 void Inputhook::SetCursorPosition(unsigned int newPosition) {
	 if (newPosition >= 0 && newPosition <= text_.length()) {
		Lock();
		cursorPos = newPosition;
		Unlock();
	 }
 }

 void Inputhook::SetSelectionPosition(unsigned int pos) {
	 if (pos < text_.length()) {
		 Lock();
		 selectPos = pos;
		 Unlock();
	 }
 }

 void Inputhook::SetSelectionLength(unsigned int length) {
	 if (length <= text_.length()) {
		 Lock();
		 selectLength = length;
		 Unlock();
	 }
 }

void Inputhook::IncreaseCursorPosition(unsigned int len) { 
	Lock();
	 SetCursorPosition(cursorPos + len); 
	 if ((textPos + GetCharacterLimit()) < cursorPos)
		 textPos = cursorPos - GetCharacterLimit();
	 Unlock();
};

void Inputhook::DecreaseCursorPosition(unsigned int len) { 
	Lock();
	SetCursorPosition(cursorPos - len); 
	 if ((cursorPos - textPos) == -1 && textPos > 0)
		 textPos -= len;
	Unlock();
}; 

unsigned int Inputhook::GetCharacterLimit() {
	return (GetXSize() / Texthook::GetTextSize("A", GetFont()).x);
}

 void Inputhook::OnDraw() {
	 Lock();
	 //Font height
	 unsigned int height[] = {10,11,18,24,10,13,7,13,10,12,8,8,7,12};
	 
	 //Current text width
	 POINT textSize = Texthook::GetTextSize(text_.substr(textPos, GetCursorPosition() - textPos).c_str(), GetFont());

	 //Draw the outline box!
	 RECT pRect  = {GetX(), GetY(), GetX() + GetXSize(), GetY() + height[GetFont()] + 4};
	 D2GFX_DrawRectangle(GetX(), GetY(), GetX() + GetXSize(), GetY() + height[GetFont()] + 4, 0, BTFull);
	 Framehook::DrawRectStub(&pRect);

	 //Draw the text in!
	 unsigned int len = text_.length() - textPos;
	 len = std::min(len, GetCharacterLimit());
	 std::wstring drawnText = text_.substr(textPos, len);

	 if (IsSelected()) {
		 drawnText.insert(GetSelectionPosition() + GetSelectionLength(), GetColorCode(TextColor::White));
		 drawnText.insert(GetSelectionPosition(), GetColorCode(TextColor::Yellow));
	 }

	 DWORD oldFont = D2WIN_SetTextSize(GetFont());
	 D2WIN_DrawText(drawnText.c_str(), GetX() + 3, GetY() + 3 + height[GetFont()], 0, 0);
	 D2WIN_SetTextSize(oldFont);

	 //Draw the cursor!
	 CursorTick();
	 if (ShowCursor() && IsActive())
		 D2GFX_DrawLine(GetX() + textSize.x + 2, GetY() + 3, GetX() + textSize.x + 2, GetY() + textSize.y, 255, 0);

	 Unlock();
 }


 bool Inputhook::OnKey(bool up, BYTE key, LPARAM lParam) {
	 if (!IsActive())
		 return false;
	 Lock();
	 bool ctrlState = ((GetKeyState(VK_LCONTROL) & 0x80) || (GetKeyState(VK_RCONTROL) & 0x80));
	 bool shiftState = ((GetKeyState(VK_LSHIFT) & 0x80) || (GetKeyState(VK_RSHIFT) & 0x80));
	 switch(key) {
		case VK_BACK:
			if (!up)
				Backspace();
		break;
		case VK_DELETE:
			if (!up && text_.length() != GetCursorPosition()) {
				Erase(GetCursorPosition(), 1);
			}
		break;
		case VK_ESCAPE:
			if (up)
				SetActive(false);
		break;
		case VK_LEFT:
			if (!up && GetCursorPosition() != 0) {
				if (shiftState) {
					if (IsSelected()) {
						if (GetSelectionPosition() == GetCursorPosition()) {
							SetSelectionPosition(GetSelectionPosition() - 1);
							SetSelectionLength(GetSelectionLength() + 1);
						} else {
							SetSelectionLength(GetSelectionLength() - 1);
						}
					} else {
						SetSelectionPosition(GetCursorPosition() - 1);
						SetSelectionLength(1);
					}
				}
				DecreaseCursorPosition(1);
			}
		break;
		case VK_RIGHT:
			if (!up && GetCursorPosition() != text_.length()) {
				if (shiftState) {
					if (IsSelected()) {
						if (GetCursorPosition() == (GetSelectionPosition() + GetSelectionLength())) {
							SetSelectionLength(GetSelectionLength() + 1);
						} else {
							SetSelectionPosition(GetSelectionPosition() + 1);
							SetSelectionLength(GetSelectionLength() - 1);
						}
					} else {
						SetSelectionPosition(GetCursorPosition());
						SetSelectionLength(1);
					}
				}
				IncreaseCursorPosition(1);
			}
		break;
		default:
			if (up)
				return true;

			if (ctrlState) {
				//Select All
				if (key == 0x41) {
					SetSelectionPosition(0);
					SetSelectionLength(text_.length());
				}
				OpenClipboard(NULL);
				//Paste
				if (key == 0x56) {
					HANDLE pHandle = GetClipboardData(CF_UNICODETEXT);
					if (!pHandle)
						return true;
					InputText((wchar_t*)GlobalLock(pHandle));
				}
				//Copy & Cut
				if (key == 0x43 || key == 0x58) {
					if (!IsSelected() || text_.length() == 0)
						return true;
					
					Lock();
					std::wstring mText = text_.substr(GetSelectionPosition(), GetSelectionLength());
			
					HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (mText.size() + 1) * sizeof(CHAR)); 
					char* szStr = (char*)GlobalLock(hGlobal);
					memcpy(szStr, mText.c_str(), mText.size() * sizeof(CHAR));
					GlobalUnlock(hGlobal);
					EmptyClipboard();
					SetClipboardData(CF_TEXT, hGlobal);

					if (key == 0x58) {
						Erase(GetSelectionPosition(), GetSelectionLength());

						ResetSelection();
					}
					Unlock();
				}
				CloseClipboard();
				return true;
			}

			BYTE layout[256];
			std::array<wchar_t, 16> buffer = { 0 };
			GetKeyboardState(layout);
			int translatedKeysCount = ToUnicode(key, lParam & 0xFF0000, layout, buffer.data(), buffer.size(), 1);
			if (translatedKeysCount == 0) {
				return false;
			}
			buffer.back() = '\0';

			InputText(buffer.data());
			break;
	 }
	 return true;
 }

 bool Inputhook::OnLeftClick(bool up, unsigned int x, unsigned int y) {
	 if (InRange(x, y)) {
		 if (up)
			 SetActive(true);
		 if (GetLeftClickHandler())
			 GetLeftClickHandler()(up, this, GetLeftClickVoid());
		 return true;
	 } else {
		 SetActive(false);
	 }
	 return false;
 }

 bool Inputhook::OnRightClick(bool up, unsigned int x, unsigned int y) {
		if (!InRange(x, y))
			if (GetLeftClickHandler())
				return GetRightClickHandler()(up, this, GetRightClickVoid());
		return false;
 }

 void Inputhook::InputText(std::wstring_view newText) {
	 Lock();

	 //If we have text selected, replace the text with the new text
	 if (IsSelected()) {
		 Replace(GetSelectionPosition(), GetSelectionLength(), newText);
		 ResetSelection();
	 //Otherwise just add the text at the cursor position.
	 } else {
		 text_.insert(GetCursorPosition(), newText);
		 IncreaseCursorPosition(newText.length());
	 }

	 Unlock();
 }

 void Inputhook::Backspace() {
	 if (GetCursorPosition() == 0)
		 return;
	 Lock();
	 if (IsSelected()) {
		 Erase(GetSelectionPosition(), GetSelectionLength());
		 ResetSelection();
	 } else {
		 text_.erase(GetCursorPosition() - 1, 1);
		 DecreaseCursorPosition(1);
		 if (textPos > 0)
			textPos -= 1;
	 }
	 Unlock();
 }

void Inputhook::Replace(unsigned int pos, unsigned int len, std::wstring_view str) {
	if ((unsigned int)pos + len > text_.length())
		return;
	Lock();
	text_.replace(pos, len, str);
	SetCursorPosition(pos + str.length());
	Unlock();
}

void Inputhook::Erase(unsigned int pos, unsigned int len) {
	if ((unsigned int)pos + len > text_.length())
		return;
	Lock();
	text_.erase(pos,len);
	SetCursorPosition(pos);
	Unlock();
}

}  // namespace Drawing
