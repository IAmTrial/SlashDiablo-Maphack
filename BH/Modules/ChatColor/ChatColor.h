#pragma once

#include <windows.h>

#include <atomic>
#include <map>
#include <string>

#include "../Module.h"

class ChatColor : public Module {
private:
	std::atomic<bool> inGame;
	std::map<std::string, unsigned int> whisperColors;
	void UpdateInGame();
public:
	ChatColor() : Module("Chat Color") {};

	void Init();

	void OnLoad();
	void LoadConfig();
	void OnGameJoin();
	void OnGameExit();
	void OnChatPacketRecv(BYTE* packet, bool *block);
};
