#pragma once

#include <windows.h>

#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "../../Config.h"
#include "../../D2Structs.h"
#include "../../Drawing/UI/UITab.h"
#include "../Module.h"

enum MaphackReveal {
	MaphackRevealGame = 0,
	MaphackRevealAct,
	MaphackRevealLevel
};

struct LevelList {
	unsigned int levelId;
	unsigned int x, y, act;
};

struct BaseSkill {
	WORD Skill;
	BYTE Level;
};

class Maphack : public Module {
	private:
		int monsterResistanceThreshold;
		int lkLinesColor;
		unsigned int revealType;
		unsigned int maxGhostSelection;
		unsigned int reloadConfig;
		bool revealedGame, revealedAct[6], revealedLevel[255];
		std::map<std::string, std::string> MonsterColors;
		std::map<std::string, std::string> SuperUniqueColors;
		std::map<std::string, std::string> MonsterLines;
		std::map<std::string, std::string> MonsterHides;
		std::map<std::string, unsigned int> TextColorMap; 
		std::map<std::string, unsigned int> monsterColors;
		std::vector<std::pair<unsigned int, unsigned int>> enhancementColors;
		std::vector<std::pair<unsigned int, unsigned int>> auraColors;
		std::map<std::string, unsigned int> missileColors;
		std::map<int, unsigned int> automapMonsterColors;
		std::map<int, unsigned int> automapSuperUniqueColors;
		std::map<int, unsigned int> automapMonsterLines;
		std::list<int> automapHiddenMonsters;
		std::list<LevelList*> automapLevels;
		std::map<std::string, Toggle> Toggles;
		Drawing::UITab* settingsTab;
		std::map<DWORD, std::vector<BaseSkill>> Skills;

	public:
	Maphack();

	void ReadConfig();
	void OnLoad();
	void OnUnload();

	void LoadConfig();

	void OnLoop();
	void OnDraw();
	void OnAutomapDraw();
	void OnGameJoin();
	void OnGamePacketRecv(BYTE* packet, bool *block);

	void ResetRevealed();
	void ResetPatches();

	void OnKey(bool up, BYTE key, LPARAM lParam, bool* block);

	void RevealGame();
	void RevealAct(int act);
	void RevealLevel(Level* level);
	void RevealRoom(Room2* room);

	static Level* GetLevel(Act* pAct, int level);
	static AutomapLayer* InitLayer(int level);
};

void Weather_Interception();
void Lighting_Interception();
void Infravision_Interception();
void __stdcall Shake_Interception(LPDWORD lpX, LPDWORD lpY);
void HoverObject_Interception();
void DiabloMessage_Interception();

void  NPCMessageLoopPatch_ASM();
void  NPCQuestMessageStartPatch_ASM();
void  NPCQuestMessageEndPatch1_ASM();
void  NPCQuestMessageEndPatch2_ASM();

