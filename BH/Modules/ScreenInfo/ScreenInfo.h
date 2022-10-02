#pragma once

#include <wchar.h>
#include <windows.h>

#include <deque>
#include <map>
#include <string>
#include <vector>
#include <utility>

#include "../../Common.h"
#include "../../Config.h"
#include "../../D2Structs.h"
#include "../../Drawing/Basic/Texthook/Texthook.h"
#include "../Module.h"

struct StateCode {
	std::string name;
	unsigned int value;
};

struct StateWarning {
	std::string name;
	ULONGLONG startTicks;
	StateWarning(std::string n, ULONGLONG ticks) : name(n), startTicks(ticks) {}
};

struct Buff {
	BYTE state;
	int index;
	BOOL isBuff;
};

class ScreenInfo : public Module {
	private:
		std::map<std::string, std::string> SkillWarnings;
		std::vector<std::string> automapInfo;
		std::map<DWORD, std::string> SkillWarningMap;
		std::deque<StateWarning*> CurrentWarnings;
		Drawing::Texthook* bhText;
		Drawing::Texthook* mpqVersionText;
		Drawing::Texthook* d2VersionText;
		DWORD gameTimer;
		DWORD endTimer;		

		int packetRequests;
		ULONGLONG warningTicks;
		ULONGLONG packetTicks;
		bool MephistoBlocked;
		bool DiabloBlocked;
		bool BaalBlocked;
		bool ReceivedQuestPacket;
		DWORD startExperience;
		int startLevel;
		std::string currentPlayer;
		DWORD currentExperience;
		int currentLevel;
		double currentExpGainPct;
		double currentExpPerSecond;
		char* currentExpPerSecondUnit;

		// used to keep track of runs over the course of a season. example cfg would look like:
		/*
		BoBarb[quickcs]: 250
		BoBarb[quickbaal]: 170
		BoBarb[mf]: 500
		*/
		//Config* cRunData;
		bool bFailedToWrite = false;
		int nTotalGames;
		std::string szGamesToLevel;
		std::string szTimeToLevel;
		std::string szLastXpGainPer;
		std::string szLastXpPerSec;
		std::string szLastGameTime;
		int aPlayerCountAverage[8];

		std::string szSavePath;
		std::string szColumnHeader;
		std::string szColumnData;

		std::map<std::string, std::string> automap;
		std::map<std::string, int> runcounter;
		std::vector<std::pair<std::string, std::string>> runDetailsColumns;
		std::map<std::string, unsigned int> runs;

		std::string SimpleGameName(const std::string& gameName);
		int	GetPlayerCount();
		void FormattedXPPerSec(char* buffer, double xpPerSec);
		std::string FormatTime(time_t t, const char* format);
		CellFile* cf;
		void* mpqH;
		BOOL manageBuffs;
		BOOL manageConv;
		int resTracker;
		BOOL cellLoaded;
		std::vector<Buff> activeBuffs;
		std::vector<BYTE> buffs;
		std::vector<const wchar_t*> buffNames;
	public:
		static std::map<std::string, Toggle> Toggles;

		ScreenInfo() :
			Module("Screen Info"), warningTicks(BHGetTickCount()), packetRequests(0),
			MephistoBlocked(false), DiabloBlocked(false), BaalBlocked(false), ReceivedQuestPacket(false),
			startExperience(0), startLevel(0), mpqH(NULL), cf(NULL), cellLoaded(false) {};

		void OnLoad();
		void LoadConfig();
		void MpqLoaded();
		void OnKey(bool up, BYTE key, LPARAM lParam, bool* block);
		void OnGameJoin();
		void OnGameExit();
	
		void OnRightClick(bool up, int x, int y, bool* block);
		void OnDraw();
		void OnOOGDraw();
		void OnAutomapDraw();
		void OnGamePacketRecv(BYTE* packet, bool *block);

		std::string ReplaceAutomapTokens(std::string& v);		
		void WriteRunTrackerData();
		void DrawPopup(const wchar_t* buffName, int x, int y);
		std::vector<std::wstring> strBreakApart(std::wstring str, wchar_t delimiter);

		static void AddDrop(UnitAny* item);
		static void AddDrop(const std::string& name, unsigned int x, unsigned int y);
};

StateCode GetStateCode(unsigned int nKey);
StateCode GetStateCode(const char* name);
long long ExpByLevel[];