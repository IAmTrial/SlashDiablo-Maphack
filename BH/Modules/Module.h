#pragma once

#include <wchar.h>
#include <windows.h>

#include <string>

#include "ModuleManager.h"

class Module {
	private:
		friend class ModuleManager;

		std::string name;
		bool active;

		void Load();
		void Unload();

	public:
		Module(std::string name);
		virtual ~Module();

		std::string GetName() { return name; };
		bool IsActive() { return active; };

		// Module Events
		virtual void OnLoad() {};
		virtual void OnUnload() {};

		virtual void LoadConfig() {};
		virtual void MpqLoaded() {};

		virtual void OnLoop() {};

		// Game Events
		virtual void OnGameJoin() {}
		virtual void OnGameExit() {};

		// Drawing Events
		virtual void OnDraw() {};
		virtual void OnAutomapDraw() {};
		virtual void OnOOGDraw() {};

		virtual void OnLeftClick(bool up, unsigned int x, unsigned int y, bool* block) {};
		virtual void OnRightClick(bool up, unsigned int x, unsigned int y, bool* block) {};
		virtual void OnKey(bool up, BYTE key, LPARAM lParam, bool* block) {};

		virtual void OnChatPacketRecv(BYTE* packet, bool* block) {};
		virtual void OnRealmPacketRecv(BYTE* packet, bool* block) {};
		virtual void OnGamePacketRecv(BYTE* packet, bool* block) {};

		virtual void OnUserInput(const wchar_t* msg, bool fromGame, bool* block) {};
		virtual void OnChatMsg(const char* user, const char* msg, bool fromGame, bool* block) {};
};
