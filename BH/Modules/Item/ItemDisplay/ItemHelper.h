
#ifndef BH_MODULES_ITEM_ITEM_DISPLAY_ITEM_HELPER_H_
#define BH_MODULES_ITEM_ITEM_DISPLAY_ITEM_HELPER_H_

#include <windows.h>

#include <string>

#include "../../../D2Structs.h"
#include "../../../MPQInit.h"
#include "Action.h"
#include "ItemInfo.h"
#include "UnitItemInfo.h"

namespace bh::modules::item {

struct ActionReplace {
	std::string key;
	std::string value;
};

struct ColorReplace {
	std::string key;
	int value;
};

struct SkillReplace {
	std::string key;
	int value;
};

extern const ColorReplace colors[16];

StatProperties *GetStatProperties(unsigned int stat);
void BuildAction(std::string *str, Action *act);
std::string ParseDescription(Action *act);
int ParsePingLevel(Action *act, const std::string& reg_string);
int ParseMapColor(Action *act, const std::string& reg_string);
void HandleUnknownItemCode(const char *code, const char *tag);
void GetItemName(UnitItemInfo *uInfo, std::string &name);
void SubstituteNameVariables(UnitItemInfo *uInfo, std::string &name, const std::string &action_name);
int GetDefense(const ItemInfo* item);
BYTE GetAffixLevel(BYTE ilvl, BYTE qlvl, BYTE mlvl);
BYTE GetRequiredLevel(UnitAny* item);
std::vector<std::string> split(const std::string &s, char delim);
unsigned int GetItemCodeIndex(char codeChar);

}  // namespace bh::modules::item

#endif  // BH_MODULES_ITEM_ITEM_DISPLAY_ITEM_HELPER_H_