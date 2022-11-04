#include "Checkhook.h"

#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#include <string>

#include "../../../Constants.h"
#include "../../../Common.h"
#include "../../../D2Ptrs.h"
#include "../../Basic/Framehook/Framehook.h"
#include "../../Basic/Texthook/Texthook.h"
#include "../../Hook.h"

namespace Drawing {

/* Basic Hook Initializer
 *		Used for drawing a checkbox on screen.
 */
Checkhook::Checkhook(
		HookVisibility visibility,
		unsigned int x,
		unsigned int y,
		bool* checked,
		const wchar_t* formatString,
		...)
				: Hook(visibility, x, y) {
	//Correctly format the string from the given arguments.
	SetTextColor(White);
	state = checked;
	SetHoverColor(Disabled);
	wchar_t buffer[4096];
	va_list arg;
	va_start(arg, formatString);
	vswprintf_s(buffer, 4096, formatString, arg);
	va_end(arg);
	text_ = buffer;
}

/* Group Hook Initializer
 *		Used in conjuction with other basic hooks to create an advanced hook.
 */
Checkhook::Checkhook(
		HookGroup* group,
		unsigned int x,
		unsigned int y,
		bool* checked,
		const wchar_t* formatString,
		...)
				: Hook(group, x, y) {
	//Correctly format the string from the given arguments.
	SetTextColor(Gold);
	state = checked;
	SetHoverColor(Tan);
	wchar_t buffer[4096];
	va_list arg;
	va_start(arg, formatString);
	vswprintf_s(buffer, 4096, formatString, arg);
	va_end(arg);
	text_ = buffer;
}

/* GetColor()
 *	Returns what color the text will be drawn.
 */
TextColor Checkhook::GetTextColor() {
	return color;
}

/* SetColor()
 *	Sets what color the text will be drawn in.
 */
void Checkhook::SetTextColor(TextColor newColor) {
	Lock();
	color = newColor;
	Unlock();
}

/* GetHoverColor()
 *	Return what color the text will be when hovered.
 */
TextColor Checkhook::GetHoverColor() {
	return hoverColor;
}

/* SetHoverColor()
 *	Sets what color to draw when hovered.
 */
void Checkhook::SetHoverColor(TextColor newHoverColor) {
	Lock();
	hoverColor = newHoverColor;
	Unlock();
}

/* GetXSize()
 *	Returns how long the text is.
 */
unsigned int Checkhook::GetXSize() {
	DWORD width, fileNo;
	DWORD oldFont = D2WIN_SetTextSize(0);
	D2WIN_GetTextWidthFileNo(text_.c_str(), &width, &fileNo);
	D2WIN_SetTextSize(oldFont);
	return width + 20; 
}

/* GetXSize()
 *	Returns how tall the text is.
 */
unsigned int Checkhook::GetYSize() {
	return 12;
}

/* IsChecked()
 *	Returns if the check is checked
 */
bool Checkhook::IsChecked() {
	return *state;
}

/* SetState()
 *	Sets the state of the check box.
 */
void Checkhook::SetState(bool checked) {
	Lock();
	*state = checked;
	Unlock();
}

/* Draw()
 *	Draws the text, must be called inside a Draw Patch
 */
void Checkhook::OnDraw() {
	if (!IsActive())
		return;

	Lock();

	Framehook::Draw(GetX(), GetY(), 12, 12, 0, BTFull);


	unsigned int drawColor = color;
	unsigned int checkColor = White;
	if (InRange(*p_D2CLIENT_MouseX, *p_D2CLIENT_MouseY) && GetHoverColor() != Disabled) {
		drawColor = hoverColor;
		checkColor = hoverColor;
	}

	if (IsChecked()) {
		Texthook::Draw(GetX() + 3, GetY() + 2, false, 0, (TextColor)checkColor, L"X");
	}

	Texthook::Draw(GetX() + 18, GetY() + 2, false, 0, (TextColor)drawColor, text_.c_str());
	Unlock();
}

/* OnLeftClick(bool up, unsigned int x, unsigned int y)
 *	Check if the text hook has been clicked on.
 */
bool Checkhook::OnLeftClick(bool up, unsigned int x, unsigned int y) {
	if (InRange(x,y)) {
		Lock();
		if (up)
			SetState(!IsChecked());
		if (GetLeftClickHandler())
			GetLeftClickHandler()(up, this, GetLeftClickVoid());
		Unlock();
		return true;
	}
	return false;
}

/* OnRightClick(bool up, unsigned int x, unsigned int y)
 *	Check if the text hook has been clicked on.
 */
bool Checkhook::OnRightClick(bool up, unsigned int x, unsigned int y) {
	if (InRange(x,y) && GetRightClickHandler()) {
		bool block = false;
		Lock();
		block = GetRightClickHandler()(up, this, GetRightClickVoid());
		Unlock();
		return block;
	}
	return false;
}

}  // namespace Drawing
