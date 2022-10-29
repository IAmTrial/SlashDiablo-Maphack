#include "Keyhook.h"

#include <wchar.h>
#include <windows.h>

#include <string>
#include <utility>

#include "../../../D2Ptrs.h"
#include "../../../Common.h"
#include "../../../Common/Input.h"
#include "../../../Common/StringUtil.h"
#include "../../../Constants.h"
#include "../../Hook.h"

namespace {

using ::common::input::VirtualKey;
using ::common::str_util::wide::FromUtf8;

}  // namespace

namespace Drawing {

/* Basic Hook Initializer
 *		Used for just drawing basics.
 */
Keyhook::Keyhook(
		HookVisibility visibility,
		unsigned int x,
		unsigned int y,
		unsigned int* key,
		std::wstring label)
				: Hook(visibility, x, y),
					label_(std::move(label)) {
	//Correctly format the string from the given arguments.
	timeout = 0;
	SetKey(key);
}

/* Group Hook Initializer
 *		Used in conjuction with other basic hooks to create an advanced hook.
 */
Keyhook::Keyhook(
		HookGroup *group,
		unsigned int x,
		unsigned int y,
		unsigned int* key,
		std::wstring label)
				: Hook(group, x, y),
					label_(std::move(label)) {
	//Correctly format the string from the given arguments.
	timeout = 0;
	SetKey(key);
}

bool Keyhook::OnLeftClick(bool up, unsigned int x, unsigned int y) {
	if (InRange(x,y)) {
		if (up) {
			if (!timeout)
				timeout = GetTickCount();
			else
				timeout = 0;
		}
		return true;
	}
	return false;
}

void Keyhook::OnDraw() {
	std::wstring text;
	bool IsInRange = InRange(*p_D2CLIENT_MouseX, *p_D2CLIENT_MouseY);
	if (!label_.empty()) {
		if(IsInRange) {
			text = std::format(L"{}{} ", label_, GetColorCode(TextColor::Tan));
		} else {
			text = std::format(L"{}{} ", label_, GetColorCode(TextColor::Gold));
		}
	}

	const VirtualKey& virtualKey = VirtualKey::GetFromCode(GetKey());
	if (timeout) {
		unsigned int time = (unsigned int)(3 - floor((GetTickCount() - timeout) / 1000.0));
		if (time <= 0) {
			timeout = 0;
		}

		text += std::format(L"{} secs", time);
	} else {
		text += std::format(L"{}", virtualKey.common_name);
	}
	DWORD size = D2WIN_SetTextSize(0);
	D2WIN_DrawText(text.c_str(), GetX(), GetY() + 10, IsInRange ? 7 : 4, 0);
	D2WIN_SetTextSize(size);
}

bool Keyhook::OnKey(bool up, BYTE kkey, LPARAM lParam) {
	if (timeout) {
		Lock();
		if (up) {
			if (kkey == VK_ESCAPE)
				kkey = 0;
			*key = (unsigned int)kkey;
			timeout = 0;
		}
		Unlock();
		return true;
	}
	return false;
}

unsigned int Keyhook::GetXSize() {
	std::wstring text;
	if (!label_.empty()) {
		text = std::format(L"{}:{}", label_, GetColorCode(TextColor::Gold));
	}
	const VirtualKey& virtualKey = VirtualKey::GetFromCode(GetKey());
	text += virtualKey.common_name;

	DWORD width, fileNo;
	DWORD oldFont = D2WIN_SetTextSize(0);
	D2WIN_GetTextWidthFileNo(text.c_str(), &width, &fileNo);
	D2WIN_SetTextSize(oldFont);
	return width;
}

unsigned int Keyhook::GetYSize() {
	return 10;
}

}  // namespace Drawing
