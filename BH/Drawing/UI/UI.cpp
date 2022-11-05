#include "UI.h"

#include <windows.h>

#include <list>
#include <string>

#include "../../BH.h"
#include "../../Common.h"
#include "../../Common/StringUtil.h"
#include "../../D2Ptrs.h"
#include "../Basic/Texthook/Texthook.h"
#include "../Basic/Framehook/Framehook.h"
#include "../Hook.h"
#include "UITab.h"

namespace Drawing {
namespace {

using ::common::str_util::ToBool;
using ::common::str_util::wide::FromUtf8;

}  // namespace

std::list<UI*> UI::UIs;
std::list<UI*> UI::Minimized;

// TODO (Mir Drualga): Remove this function once UTF-8 is confirmed
UI::UI(std::string name, unsigned int xSize, unsigned int ySize)
		: UI(FromUtf8(name), xSize, ySize) {}

UI::UI(std::wstring name, unsigned int xSize, unsigned int ySize)
		: name_(std::move(name)) {
	InitializeCriticalSection(&crit);
	SetXSize(xSize);
	SetYSize(ySize);
	std::wstring path = FromUtf8(BH::path) + L"UI.ini";
	int x = GetPrivateProfileIntW(name_.c_str(), L"X", 0, path.c_str());
	SetX(x);
	int y = GetPrivateProfileIntW(name_.c_str(), L"Y", 0, path.c_str());
	SetY(y);
	int minX = GetPrivateProfileIntW(name_.c_str(), L"minimizedX", MINIMIZED_X_POS, path.c_str());
	SetMinimizedX(minX);
	int minY = GetPrivateProfileIntW(name_.c_str(), L"minimizedY", MINIMIZED_Y_POS, path.c_str());
	SetMinimizedY(minY);
	wchar_t activeStr[20];
	GetPrivateProfileStringW(name_.c_str(), L"Minimized", L"true", activeStr, 20, path.c_str());
	if (ToBool(activeStr).value_or(false) || wcscmp(activeStr, L"1") == 0) {
		SetMinimized(true);
		Minimized.push_back(this);
	} else {
		SetMinimized(false);
	}
	SetActive(false);
	zOrder = UIs.size();
	UIs.push_back(this);
}
UI::~UI() {
	Lock();
	std::wstring path = FromUtf8(BH::path) + L"UI.ini";
	WritePrivateProfileStringW(name_.c_str(), L"X", std::to_wstring(GetX()).c_str(), path.c_str());
	WritePrivateProfileStringW(name_.c_str(), L"Y", std::to_wstring(GetY()).c_str(), path.c_str());
	WritePrivateProfileStringW(name_.c_str(), L"Minimized", IsMinimized() ? L"true" : L"false", path.c_str());
	WritePrivateProfileStringW(name_.c_str(), L"minimizedX", std::to_wstring(GetMinimizedX()).c_str(), path.c_str());
	WritePrivateProfileStringW(name_.c_str(), L"minimizedY", std::to_wstring(GetMinimizedY()).c_str(), path.c_str());

	while (!Tabs.empty()) {
		delete (*Tabs.begin());
	}
		
	UIs.remove(this);
	if (IsMinimized())
		Minimized.remove(this);

	Unlock();
	DeleteCriticalSection(&crit);
}

void UI::SetX(unsigned int newX) {
	if (newX >= 0 && newX <= Hook::GetScreenWidth()) {
		Lock();
		x = newX;
		Unlock();
	}
}

void UI::SetY(unsigned int newY) {
	if (newY >= 0 && newY <= Hook::GetScreenHeight()) {
		Lock();
		y = newY;
		Unlock();
	}
}

void UI::SetXSize(unsigned int newXSize) {
	if (newXSize >= 0 && newXSize <= (Hook::GetScreenHeight() - GetX())) {
		Lock();
		xSize = newXSize;
		Unlock();
	}
}

void UI::SetYSize(unsigned int newYSize) {
	if (newYSize >= 0 && newYSize <= (Hook::GetScreenHeight() - GetX())) {
		Lock();
		ySize = newYSize;
		Unlock();
	}
}

void UI::SetMinimizedX(unsigned int newX) {
	// If we limit by width injecting on load won't work
	// Just don't give invalid params :)
	// if (newX >= 0 && newX <= Hook::GetScreenWidth()) {
	if (newX >= 0) {
		Lock();
		minimizedX = newX;
		Unlock();
	}
}

void UI::SetMinimizedY(unsigned int newY) {
	// If we limit by height injecting on load won't work
	// Just don't give invalid params :)
	// if (newY >= 0 && newY <= Hook::GetScreenHeight()) {
	if (newY >= 0) {
		Lock();
		minimizedY = newY;
		Unlock();
	}
}

void UI::OnDraw() {
	if (!IsVisible()) return;
	EnsureInBounds();
	if (IsMinimized()) {
		int n = 0;
		for (std::list<UI*>::iterator it = Minimized.begin(); it != Minimized.end(); it++, n++)
			if ((*it) == this)
				break;

		int xSize = Texthook::GetTextSize(name_.c_str(), 0).x + 8;

		if (IsDragged()) {
			int newX = (*p_D2CLIENT_MouseX) - dragX;
			int newY = (*p_D2CLIENT_MouseY) - dragY;

			if (newX < 0)
				newX = 0;

			if ((newX + xSize + 2) > (int)Hook::GetScreenWidth())
				newX = Hook::GetScreenWidth() - xSize - 2;

			if (newY < 2)
				newY = 2;

			if ((newY + TITLE_BAR_HEIGHT) > (int)Hook::GetScreenHeight())
				newY = Hook::GetScreenHeight() - TITLE_BAR_HEIGHT;

			*p_D2CLIENT_MouseX = newX + dragX;
			*p_D2CLIENT_MouseY = newY + dragY;
			SetMinimizedX(newX);
			SetMinimizedY(newY);
		}
		int yPos = GetMinimizedY() - (n * (TITLE_BAR_HEIGHT + 4));
		int inPos = InPos((*p_D2CLIENT_MouseX), (*p_D2CLIENT_MouseY), GetMinimizedX(), yPos, xSize, TITLE_BAR_HEIGHT);
		Framehook::Draw(GetMinimizedX(), yPos, xSize, TITLE_BAR_HEIGHT, 0, BTOneHalf);
		Texthook::Draw(GetMinimizedX() + 4, yPos + 3, false, 0, (inPos?Silver:White), name_.c_str());
	} else {
		if (IsDragged()) {
			int newX = (*p_D2CLIENT_MouseX) - dragX;
			int newY = (*p_D2CLIENT_MouseY) - dragY;

			if (newX < 0)
				newX = 0;

			if ((newX + GetXSize() + 2) > Hook::GetScreenWidth())
				newX = Hook::GetScreenWidth() - GetXSize() - 2;

			if (newY < 2)
				newY = 2;

			if ((newY + GetYSize()) > Hook::GetScreenHeight())
				newY = Hook::GetScreenHeight() - GetYSize();

			*p_D2CLIENT_MouseX = newX + dragX;
			*p_D2CLIENT_MouseY = newY + dragY;
			SetX(newX);
			SetY(newY);
		}
		Framehook::Draw(GetX(), GetY(), GetXSize(), GetYSize(), 0, (IsActive()?BTNormal:BTOneHalf));
		Framehook::Draw(GetX(), GetY(), GetXSize(), TITLE_BAR_HEIGHT, 0, BTNormal);
		Texthook::Draw(GetX() + 4, GetY () + 3, false, 0, InTitle((*p_D2CLIENT_MouseX), (*p_D2CLIENT_MouseY))?Silver:White, name_.c_str());
		for (std::list<UITab*>::iterator it = Tabs.begin(); it != Tabs.end(); it++)
			(*it)->OnDraw();
	}
}

void UI::EnsureInBounds() {
	if (IsMinimized()) {
		if (GetMinimizedX() < 0) {
			SetMinimizedX(0);
		}
		if (GetMinimizedX() + GetXSize() > Hook::GetScreenWidth()) {
			SetMinimizedX(Hook::GetScreenWidth() - GetXSize());
		}
		if (GetMinimizedY() < 0) {
			SetMinimizedY(0);
		}
		if (GetMinimizedY() + TITLE_BAR_HEIGHT > Hook::GetScreenHeight()) {
			SetMinimizedY(Hook::GetScreenHeight() - TITLE_BAR_HEIGHT);
		}
	}
	else {
		if (GetX() < 0) {
			SetX(0);
		}
		if(GetX() + GetXSize() > Hook::GetScreenWidth()) {
			SetX(Hook::GetScreenWidth() - GetXSize());
		}
		if (GetY() < 0) {
			SetY(0);
		}
		if (GetY() + GetYSize() > Hook::GetScreenHeight()) {
			SetY(Hook::GetScreenHeight() - GetYSize());
		}
	}
}

void UI::SetDragged(bool state, bool write_file) {
	Lock(); 
	dragged = state; 
	if (!state && write_file) {
		std::wstring path = FromUtf8(BH::path) + L"UI.ini";
		WritePrivateProfileStringW(name_.c_str(), L"X", std::to_wstring(GetX()).c_str(), path.c_str());
		WritePrivateProfileStringW(name_.c_str(), L"Y", std::to_wstring(GetY()).c_str(), path.c_str());
		WritePrivateProfileStringW(name_.c_str(), L"minimizedX", std::to_wstring(GetMinimizedX()).c_str(), path.c_str());
		WritePrivateProfileStringW(name_.c_str(), L"minimizedY", std::to_wstring(GetMinimizedY()).c_str(), path.c_str());
	}
	Unlock(); 
}

void UI::SetDragged(bool state) {
    SetDragged(state, false);
}

void UI::SetVisible(bool newState) {
	visible = newState;
}

void UI::SetMinimized(bool newState) { 
	if (newState == minimized) 
		return; 
	Lock();  
	if (newState) {
		Minimized.push_back(this);
		BH::config->Write();
	} else
		Minimized.remove(this); 
	minimized = newState; 
	std::wstring path = FromUtf8(BH::path) + L"UI.ini";
	WritePrivateProfileStringW(name_.c_str(), L"Minimized", newState ? L"true" : L"false", path.c_str());
	Unlock(); 
};

bool UI::OnLeftClick(bool up, unsigned int mouseX, unsigned int mouseY) {
	if (IsMinimized()) {
		int n = 0;
		for (std::list<UI*>::iterator it = Minimized.begin(); it != Minimized.end(); it++, n++)
			if ((*it) == this)
				break;
		int yPos = GetMinimizedY() - (n * (TITLE_BAR_HEIGHT + 4));
		int xSize = Texthook::GetTextSize(name_.c_str(), 0).x + 8;
		int inPos = InPos((*p_D2CLIENT_MouseX), (*p_D2CLIENT_MouseY), GetMinimizedX(), yPos, xSize, TITLE_BAR_HEIGHT);
		if (inPos /*&& GetAsyncKeyState(VK_CONTROL)*/) 
		{
			if(GetAsyncKeyState(VK_CONTROL))
			{
				if (up) {
					SetMinimized(false);
					Sort(this);
				}
				return true;
			}
			else if (GetAsyncKeyState(VK_SHIFT) && !up) {
				SetDragged(true);
				dragX = mouseX - GetMinimizedX();
				dragY = mouseY - GetMinimizedY();
				startX = mouseX;
				startY = mouseY;
			}
			else
			{
				SetDragged(false, true);
				if(!up) {
					PrintText(7, "CTRL-click to open settings" );
					PrintText(7, "Shift-drag to move" );
				}
			}
			return true;
		}
	}
	if (InTitle(mouseX, mouseY) && !IsMinimized()) {
		if (!up) 
		{
			SetDragged(true);
			dragX = mouseX - GetX();
			dragY = mouseY - GetY();
			startX = mouseX;
			startY = mouseY;
		} 
		else
		{
			SetDragged(false, true);
			if( startX == mouseX && startY == mouseY && GetAsyncKeyState(VK_CONTROL) )
			{
				PrintText(135, "Right Click to Close" );
			}
		}
		SetActive(true);
		Sort(this);
		return true;
	} else if (InWindow(mouseX, mouseY) && !IsMinimized()) {
		SetActive(true);
		Sort(this);
		if (up) {
			for (std::list<UITab*>::iterator it = Tabs.begin(); it != Tabs.end(); it++) {
				if ((*it)->IsHovering(mouseX, mouseY)) {
					SetCurrentTab(*it);
					return true;
				}
			}
		}
		return true;
	}
	SetActive(false);
	SetDragged(false, false);
	return false;
}

bool UI::OnRightClick(bool up, unsigned int mouseX, unsigned int mouseY) {
	if (InTitle(mouseX, mouseY) && !IsMinimized()) {
		if (up) 
			SetMinimized(true);
		return true;
	}
	return false;
}

void UI::Sort(UI* zero) {
	int zOrder = 1;
	for (std::list<UI*>::iterator it = UIs.begin(); it != UIs.end(); it++, zOrder++) {
		if ((*it) == zero) {
			(*it)->SetZOrder(0);
			zOrder--;
		} else {
			(*it)->SetZOrder(zOrder);
		}
	}
}

/* UI Helper functions 
 *	Static functions to aid in sending events to UIs.
 */
 
bool ZSortClick (UI* one, UI* two) {
	return one->GetZOrder() < two->GetZOrder();
}
bool ZSortDraw (UI* one, UI* two) {
	return one->GetZOrder() > two->GetZOrder();
}

void UI::Draw() {
	UIs.sort(ZSortDraw);
	for (std::list<UI*>::iterator it = UIs.begin(); it!=UIs.end(); ++it) {
			(*it)->Lock();
			(*it)->OnDraw();
			(*it)->Unlock();
	}
}	

bool UI::LeftClick(bool up, unsigned int mouseX, unsigned int mouseY) {
	UIs.sort(ZSortClick);
	for (std::list<UI*>::iterator it = UIs.begin(); it!=UIs.end(); ++it) {
		(*it)->Lock();
		if ((*it)->OnLeftClick(up, mouseX, mouseY)) {
			(*it)->Unlock();
			return true;
		}
		(*it)->Unlock();
	}
	return false;
}

bool UI::RightClick(bool up, unsigned int mouseX, unsigned int mouseY) {
	UIs.sort(ZSortClick);
	for (std::list<UI*>::iterator it = UIs.begin(); it!=UIs.end(); ++it) {
		(*it)->Lock();
		if ((*it)->OnRightClick(up, mouseX, mouseY)) {
			(*it)->Unlock();
			return true;
		}
		(*it)->Unlock();
	}
	return false;
}

}  // namespace Drawing
