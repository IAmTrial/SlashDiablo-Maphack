#pragma once

#include <windows.h>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../../Config.h"
#include "../../Drawing/UI/UITab.h"
#include "../../Mustache.h"
#include "../../JSONObject.h"
#include "../Module.h"

#define MAX_FUNCTION 23

struct UnitAny;

struct IdNamePair{
	int id;
	std::string name;
};

class StashExport : public Module {
private:

	std::string dfltExprt;
	std::map<std::string, std::string> mustaches;
	unsigned int exportGear;
	unsigned int exportType;
	static UnitAny* viewingUnit;
	Drawing::UITab* settingsTab;
	std::vector<std::string> options;

	static void fillStats(JSONArray* statsArray, JSONObject *itemDef, UnitAny *pItem, std::string codeKey, std::string paramKey, std::string minKey, std::string maxKey, int maxProps);
public:
	static std::map<std::string, Toggle> Toggles;
	static std::map<std::string, std::unique_ptr<Mustache::AMustacheTemplate>> MustacheTemplates;

	StashExport() : Module("StashExport"), exportType(0) { dfltExprt = "json"; };

	void OnLoad();
	void OnUnload();

	void LoadConfig();

	void OnLoop();
	void OnKey(bool up, BYTE key, LPARAM lParam, bool* block);
	void WriteStash();
	std::map<std::string, Toggle>* GetToggles() { return &Toggles; }
	static void GetItemInfo(UnitAny* pItem, JSONObject* pBuffer);
	static JSONObject* getStatEntry(WORD statId, WORD statId2, DWORD statVal, DWORD min, DWORD max);
};

void ItemDesc_Interception();

std::function<JSONObject*(UnitAny*, JSONObject*, JSONObject*, JSONElement*, int, int)> STAT_FUNCTIONS[];
const char* QUALITY_NAMES[];
IdNamePair NODE_PAGES[];
IdNamePair CLASS_NAMES[];
IdNamePair SKILL_TABS[];
