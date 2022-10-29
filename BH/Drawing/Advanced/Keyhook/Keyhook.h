#pragma once

#include <windows.h>

#include <string>

#include "../../Hook.h"

namespace Drawing {

class Keyhook : public Hook {
	private:
		unsigned int* key;//Pointer to the current key
		std::wstring label_;//Label to describe the hotkey
		unsigned int timeout;//Timeout to change hotkey if clicked
	public:
		// TODO (Mir Drualga): Remove this constructor once UTF-8 use is confirmed.
		Keyhook(
				HookVisibility visibility,
				unsigned int x,
				unsigned int y,
				unsigned int* key,
				std::string label)
						: Keyhook(std::move(visibility), x, y, key, FromUtf8(label)) {}
		// TODO (Mir Drualga): Remove this constructor once UTF-8 use is confirmed.
		Keyhook(
				HookGroup* group,
				unsigned int x,
				unsigned int y,
				unsigned int* key,
				std::string label)
						: Keyhook(group, x, y, key, FromUtf8(label)) {}
		//Two Hook Initializations; one for basic hooks, one for grouped hooks.
		Keyhook(
				HookVisibility visibility,
				unsigned int x,
				unsigned int y,
				unsigned int* key,
				std::wstring label);
		Keyhook(
				HookGroup* group,
				unsigned int x,
				unsigned int y,
				unsigned int* key,
				std::wstring label);

		unsigned int GetKey() { return *key; };
		void SetKey(unsigned int* newKey) { Lock(); key = newKey; Unlock(); };

		bool OnLeftClick(bool up, unsigned int x, unsigned int y);
		void OnDraw();
		bool OnKey(bool up, BYTE key, LPARAM lParam);

		unsigned int GetXSize();
		unsigned int GetYSize();
};

}  // namespace Drawing
