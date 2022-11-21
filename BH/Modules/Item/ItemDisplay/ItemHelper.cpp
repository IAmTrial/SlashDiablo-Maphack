
#include "ItemHelper.h"

#include <windows.h>

#include <algorithm>
#include <map>
#include <regex>
#include <string>

#include "bh/d2/d2lang/function/universal/get_locale_text.hpp"
#include "../../../BH.h"
#include "../../../Common.h"
#include "../../../Constants.h"
#include "../../../D2Helpers.h"
#include "../../../D2Ptrs.h"
#include "../../../D2Structs.h"
#include "../../../MPQInit.h"
#include "Action.h"
#include "ColorConstants.h"
#include "GemRune.h"
#include "ItemInfo.h"
#include "LookupCache.h"
#include "UnitItemInfo.h"

namespace bh::modules::item {
namespace {

namespace d2lang = ::bh::d2::d2lang;

}  // namespace

// All colors here must also be defined in MAP_COLOR_REPLACEMENTS
#define COLOR_REPLACEMENTS	\
  {"WHITE", "\377c0"},		\
  {"RED", "\377c1"},			\
  {"GREEN", "\377c2"},		\
  {"BLUE", "\377c3"},		\
  {"GOLD", "\377c4"},		\
  {"GRAY", "\377c5"},		\
  {"BLACK", "\377c6"},		\
  {"TAN", "\377c7"},			\
  {"ORANGE", "\377c8"},		\
  {"YELLOW", "\377c9"},		\
  {"PURPLE", "\377c;"},		\
  {"DARK_GREEN", "\377c:"},	\
  {"CORAL", "\377c\x06"},		\
  {"SAGE", "\377c\x07"},		\
  {"TEAL", "\377c\x09"},		\
  {"LIGHT_GRAY", "\xFF" "c\x0C"}

#define MAP_COLOR_WHITE     0x20

const ColorReplace colors[16] = {
  {"WHITE", 0x20},
  {"RED", 0x0A},
  {"GREEN", 0x84},
  {"BLUE", 0x97},
  {"GOLD", 0x0D},
  {"GRAY", 0xD0},
  {"BLACK", 0x00},
  {"TAN", 0x5A},
  {"ORANGE", 0x60},
  {"YELLOW", 0x0C},
  {"PURPLE", 0x9B},
  {"DARK_GREEN", 0x76},
  {"CORAL", 0x66},
  {"SAGE", 0x82},
  {"TEAL", 0xCB},
  {"LIGHT_GRAY", 0xD6}
};

void BuildAction(std::string *str, Action *act) {
  act->name = std::string(str->c_str());

  // upcase all text in a %replacement_string%
  // for some reason \w wasn't catching _, so I added it to the groups
  std::regex replace_reg(
      R"(^(?:(?:%[^%]*%)|[^%])*%((?:\w|_|-)*?[a-z]+?(?:\w|_|-)*?)%)",
      std::regex_constants::ECMAScript);
  std::smatch replace_match;
  while (std::regex_search(act->name, replace_match, replace_reg)) {
    auto offset = replace_match[1].first - act->name.begin();
    std::transform(
        replace_match[1].first,
        replace_match[1].second,
        act->name.begin() + offset,
        toupper
        );
  }

  // new stuff:
  act->borderColor = ParseMapColor(act, "BORDER");
  act->colorOnMap = ParseMapColor(act, "MAP");
  act->dotColor = ParseMapColor(act, "DOT");
  act->pxColor = ParseMapColor(act, "PX");
  act->lineColor = ParseMapColor(act, "LINE");
  act->notifyColor = ParseMapColor(act, "NOTIFY");
  act->pingLevel = ParsePingLevel(act, "TIER");
  act->description = ParseDescription(act);

  size_t noTracking = act->name.find("%NOTRACK%");
  if (noTracking != std::string::npos) {
    act->name.replace(noTracking, 9, "");
    act->noTracking = true;
  }

  // legacy support:
  size_t map = act->name.find("%MAP%");
  if (map != std::string::npos) {
    int mapColor = MAP_COLOR_WHITE;
    size_t lastColorPos = 0;
    for (int n = 0; n < sizeof(colors) / sizeof(colors[0]); n++) {
      size_t pos = act->name.find("%" + colors[n].key + "%");
      if (pos != std::string::npos && pos < map && pos >= lastColorPos) {
        mapColor = colors[n].value;
        lastColorPos = pos;
      }
    }

    act->name.replace(map, 5, "");
    act->colorOnMap = mapColor;
    if (act->borderColor == UNDEFINED_COLOR)
      act->borderColor = act->colorOnMap;
  }

  size_t done = act->name.find("%CONTINUE%");
  if (done != std::string::npos) {
    act->name.replace(done, 10, "");
    act->stopProcessing = false;
  }
}

void SubstituteNameVariables(UnitItemInfo *uInfo, std::string &name, const std::string &action_name) {
  char origName[128], sockets[4], code[4], ilvl[4], alvl[4], craft_alvl[4], runename[16] = "", runenum[4] = "0";
  char gemtype[16] = "", gemlevel[16] = "", sellValue[16] = "", statVal[16] = "";
  char lvlreq[4], wpnspd[4], rangeadder[4];

  UnitAny *item = uInfo->item;
  ItemText *txt = D2COMMON_GetItemText(item->dwTxtFileNo);
  char *szCode = txt->szCode;
  code[0] = szCode[0];
  code[1] = szCode[1];
  code[2] = szCode[2];
  code[3] = '\0';
  auto ilvl_int = item->pItemData->dwItemLevel;
  auto alvl_int = GetAffixLevel((BYTE)item->pItemData->dwItemLevel, (BYTE)uInfo->attrs->qualityLevel, uInfo->attrs->magicLevel);
  auto clvl_int = D2COMMON_GetUnitStat(D2CLIENT_GetPlayerUnit(), STAT_LEVEL, 0);
  sprintf_s(sockets, "%d", D2COMMON_GetUnitStat(item, STAT_SOCKETS, 0));
  sprintf_s(ilvl, "%d", ilvl_int);
  sprintf_s(alvl, "%d", alvl_int);
  sprintf_s(craft_alvl, "%d", GetAffixLevel((BYTE)(ilvl_int/2+clvl_int/2), (BYTE)uInfo->attrs->qualityLevel, uInfo->attrs->magicLevel));
  sprintf_s(origName, "%s", name.c_str());

  sprintf_s(lvlreq, "%d", GetRequiredLevel(uInfo->item));
  sprintf_s(wpnspd, "%d", txt->speed); //Add these as matchable stats too, maybe?
  sprintf_s(rangeadder, "%d", txt->rangeadder);

  UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
  if (pUnit && txt->fQuest == 0) {
    sprintf_s(sellValue, "%d", D2COMMON_GetItemPrice(pUnit, item, D2CLIENT_GetDifficulty(), (DWORD)D2CLIENT_GetQuestInfo(), 0x201, 1));
  }

  if (IsRune(uInfo->attrs)) {
    sprintf_s(runenum, "%d", RuneNumberFromItemCode(code));
    sprintf_s(runename, name.substr(0, name.find(' ')).c_str());
  } else if (IsGem(uInfo->attrs)) {
    sprintf_s(gemlevel, "%s", GetGemLevelString(GetGemLevel(uInfo->attrs)));
    sprintf_s(gemtype, "%s", GetGemTypeString(GetGemType(uInfo->attrs)));
  }

  std::string baseName =
      UnicodeToAnsi(d2lang::universal::GetLocaleText(txt->nLocaleTxtNo));

  ActionReplace replacements[] = {
    {"NAME", origName},
    {"BASENAME", baseName},
    {"SOCKETS", sockets},
    {"RUNENUM", runenum},
    {"RUNENAME", runename},
    {"GEMLEVEL", gemlevel},
    {"GEMTYPE", gemtype},
    {"ILVL", ilvl},
    {"ALVL", alvl},
    {"CRAFTALVL", craft_alvl},
    {"LVLREQ", lvlreq},
    {"WPNSPD", wpnspd},
    {"RANGE", rangeadder},
    {"CODE", code},
    {"NL", "\n"},
    {"PRICE", sellValue},
    COLOR_REPLACEMENTS
  };
  name.assign(action_name);
  for (int n = 0; n < sizeof(replacements) / sizeof(replacements[0]); n++) {

    // Revert to non-glide colors here
    if (*p_D2GFX_VideoMode != VIDEO_MODE_GLIDE) {
      if (replacements[n].key == "CORAL") {
        replacements[n].value = "\377c1"; // red
      } else if (replacements[n].key == "SAGE") {
        replacements[n].value = "\377c2"; // green
      } else if (replacements[n].key == "TEAL") {
        replacements[n].value = "\377c3"; // blue
      } else if (replacements[n].key == "LIGHT_GRAY") {
        replacements[n].value = "\377c5"; // gray
      }
    }

    while (name.find("%" + replacements[n].key + "%") != std::string::npos) {
      name.replace(name.find("%" + replacements[n].key + "%"), replacements[n].key.length() + 2, replacements[n].value);
    }
  }

  // stat replacements
  if (name.find("%STAT-") != std::string::npos) {
    std::regex stat_reg("%STAT-([0-9]{1,4})%", std::regex_constants::ECMAScript);
    std::smatch stat_match;

    while (std::regex_search(name, stat_match, stat_reg)) {
      int stat = stoi(stat_match[1].str(), nullptr, 10);
      statVal[0] = '\0';
      if (stat <= (int)STAT_MAX) {
        auto value = D2COMMON_GetUnitStat(item, stat, 0);
        // Hp and mana need adjusting
        if (stat == 7 || stat == 9)
          value /= 256;
        sprintf_s(statVal, "%d", value);
      }
      name.replace(
          stat_match.prefix().length(),
          stat_match[0].length(), statVal);
    }
  }
}

std::string ParseDescription(Action *act) {
  size_t l_idx = act->name.find("{");
  size_t r_idx = act->name.find("}");
  if (l_idx == std::string::npos || r_idx == std::string::npos || l_idx > r_idx) return "";
  size_t start_idx = l_idx + 1;
  size_t len = r_idx - start_idx;
  std::string desc_string = act->name.substr(start_idx, len);
  act->name.replace(l_idx, len+2, "");
  return desc_string;
}

int ParseMapColor(Action *act, const std::string& key_string) {
  std::regex pattern("%" + key_string + "-([a-f0-9]{1,4})%",
    std::regex_constants::ECMAScript | std::regex_constants::icase);
  int color = UNDEFINED_COLOR;
  std::smatch the_match;

  if (std::regex_search(act->name, the_match, pattern)) {
    color = stoi(the_match[1].str(), nullptr, 16);
    act->name.replace(
        the_match.prefix().length(),
        the_match[0].length(), "");
  }
  return color;
}

int ParsePingLevel(Action *act, const std::string& key_string) {
  std::regex pattern("%" + key_string + "-([0-9])%",
    std::regex_constants::ECMAScript | std::regex_constants::icase);
  int ping_level = 0;
  std::smatch the_match;

  if (std::regex_search(act->name, the_match, pattern)) {
    ping_level = stoi(the_match[1].str());
    act->name.replace(
        the_match.prefix().length(),
        the_match[0].length(), "");
  }
  return ping_level;
}

int GetDefense(const ItemInfo *item) {
  int def = item->defense;
  for (std::vector<ItemProperty>::const_iterator prop = item->properties.begin(); prop < item->properties.end(); prop++) {
    if (prop->stat == STAT_ENHANCEDDEFENSE) {
      def *= (prop->value + 100);
      def /= 100;
    }
  }
  return def;
}

void HandleUnknownItemCode(const char *code, const char *tag) {
  static std::map<std::string, int> UnknownItemCodes;

  // If the MPQ files arent loaded yet then this is expected
  if (!IsInitialized()){
    return;
  }

  // Avoid spamming endlessly
  if (UnknownItemCodes.size() > 10 || (*BH::MiscToggles2)["Allow Unknown Items"].state) {
    return;
  }
  if (UnknownItemCodes.find(code) == UnknownItemCodes.end()) {
    PrintText(1, "Unknown item code %s: %c%c%c\n", tag, code[0], code[1], code[2]);
    UnknownItemCodes[code] = 1;
  }
}

void GetItemName(UnitItemInfo *uInfo, std::string &name) {
  std::string new_name = item_name_cache.Get(uInfo, name);
  name.assign(new_name);
}

StatProperties *GetStatProperties(unsigned int stat) {
  return AllStatList.at(stat);
}

// Helper function to get a list of strings
std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string item;
  while (getline(ss, item, delim)) {
    result.push_back(item);
  }
  return result;
}

BYTE GetAffixLevel(BYTE ilvl, BYTE qlvl, BYTE mlvl) {
  if (ilvl > 99) {
    ilvl = 99;
  }
  if (qlvl > ilvl) {
    ilvl = qlvl;
  }
  if (mlvl > 0) {
    return ilvl + mlvl > 99 ? 99 : ilvl + mlvl;
  }
  return ((ilvl) < (99 - ((qlvl)/2)) ? (ilvl) - ((qlvl)/2) : (ilvl) * 2 - 99);
}

// Returns the (lowest) level requirement (for any class) of an item
BYTE GetRequiredLevel(UnitAny* item) {
  // Some crafted items can supposedly go above 100, but it's practically the same as 100
  BYTE rlvl = 100;

  // The unit for which the required level is calculated
  UnitAny* character = D2CLIENT_GetPlayerUnit();

  // Extra checks for these as they can have charges
  if (item->pItemData->dwQuality == ITEM_QUALITY_RARE || item->pItemData->dwQuality == ITEM_QUALITY_MAGIC) {

    // Save the original class of the character (0-6)
    DWORD temp = character->dwTxtFileNo;

    // Pretend to be every class once, use the lowest req lvl (for charged items)
    for (DWORD i = 0; i < 7; i++) {

      character->dwTxtFileNo = i;
      BYTE temprlvl = (BYTE)D2COMMON_GetItemLevelRequirement(item, character);

      if (temprlvl < rlvl) {

        rlvl = temprlvl;
        //Only one class will have a lower req than the others, so if a lower one is found we can stop
        if (i > 0) { break; }
      }
    }
    // Go back to being original class
    character->dwTxtFileNo = temp;
  } else {
    rlvl = (BYTE)D2COMMON_GetItemLevelRequirement(item, character);
  }

  return rlvl;
}

unsigned int GetItemCodeIndex(char codeChar) {
  // Characters '0'-'9' map to 0-9, and a-z map to 10-35
  return codeChar - (codeChar < 90 ? 48 : 87);
}

}  // namespace bh::modules::item
