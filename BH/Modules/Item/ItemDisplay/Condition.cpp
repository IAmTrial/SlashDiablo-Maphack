
#include "Condition.h"

#include <windows.h>

#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "../../../BH.h"
#include "../../../D2Ptrs.h"
#include "../../../Common.h"
#include "../../../Common/StringUtil.h"
#include "../../../Constants.h"
#include "../Item.h"
#include "GemRune.h"
#include "ItemHelper.h"
#include "ItemInfo.h"
#include "UnitItemInfo.h"

namespace bh::modules::item {
namespace {

using ::common::str_util::ToBool;
using ::common::str_util::Trim;

#define PLAYER_CLASSIC 0
#define PLAYER_XP 1

enum Operation {
  EQUAL,
  GREATER_THAN,
  LESS_THAN,
  NONE
};

static BYTE GetOperation(std::string *op) {

  if (op->length() < 1) {
    return NONE;
  } else if ((*op)[0] == '=') {
    return EQUAL;
  } else if ((*op)[0] == '<') {
    return LESS_THAN;
  } else if ((*op)[0] == '>') {
    return GREATER_THAN;
  }
  return NONE;
}

static bool IntegerCompare(unsigned int Lvalue, BYTE operation, unsigned int Rvalue) {
  switch (operation) {
  case EQUAL:
    return Lvalue == Rvalue;
  case GREATER_THAN:
    return Lvalue > Rvalue;
  case LESS_THAN:
    return Lvalue < Rvalue;
  default:
    return false;
  }
}

// All the types able to be combined with the + operator
#define COMBO_STATS					\
	{"LIFE", STAT_MAXHP},			\
	{"MANA", STAT_MAXMANA},			\
	{"STR", STAT_STRENGTH},			\
	{"DEX", STAT_DEXTERITY},		\
	{"CRES", STAT_COLDRESIST},		\
	{"FRES", STAT_FIRERESIST},		\
	{"LRES", STAT_LIGHTNINGRESIST},	\
	{"PRES", STAT_POISONRESIST},	\
	{"MINDMG", STAT_MINIMUMDAMAGE},	\
	{"MAXDMG", STAT_MAXIMUMDAMAGE},	\

SkillReplace skills[] = { 
	COMBO_STATS 
};

}  // namespace

BYTE LastConditionType;
const std::string Condition::tokenDelims = "<=>";

// Implements the shunting-yard algorithm to evaluate condition expressions
// Returns a vector of conditions in Reverse Polish Notation
void Condition::ProcessConditions(std::vector<Condition*> &inputConditions, std::vector<Condition*> &processedConditions) {
  std::vector<Condition*> conditionStack;
  unsigned int size = inputConditions.size();
  for (unsigned int c = 0; c < size; c++) {
    Condition *input = inputConditions[c];
    if (input->conditionType == CT_Operand) {
      processedConditions.push_back(input);
    } else if (input->conditionType == CT_BinaryOperator || input->conditionType == CT_NegationOperator) {
      bool go = true;
      while (go) {
        if (conditionStack.size() > 0) {
          Condition *stack = conditionStack.back();
          if ((stack->conditionType == CT_NegationOperator || stack->conditionType == CT_BinaryOperator) &&
            input->conditionType == CT_BinaryOperator) {
            conditionStack.pop_back();
            processedConditions.push_back(stack);
          } else {
            go = false;
          }
        } else {
          go = false;
        }
      }
      conditionStack.push_back(input);
    } else if (input->conditionType == CT_LeftParen) {
      conditionStack.push_back(input);
    } else if (input->conditionType == CT_RightParen) {
      bool foundLeftParen = false;
      while (conditionStack.size() > 0 && !foundLeftParen) {
        Condition *stack = conditionStack.back();
        conditionStack.pop_back();
        if (stack->conditionType == CT_LeftParen) {
          foundLeftParen = true;
          break;
        } else {
          processedConditions.push_back(stack);
        }
      }
      if (!foundLeftParen) {
        // TODO: find a way to report error
        return;
      }
    }
  }
  while (conditionStack.size() > 0) {
    Condition *next = conditionStack.back();
    conditionStack.pop_back();
    if (next->conditionType == CT_LeftParen || next->conditionType == CT_RightParen) {
      // TODO: find a way to report error
      break;
    } else {
      processedConditions.push_back(next);
    }
  }
}

// Insert extra AND operators to stay backwards compatible with old config
// that implicitly ANDed all conditions
void Condition::AddOperand(std::vector<Condition*> &conditions, Condition *cond) {
  if (LastConditionType == CT_Operand || LastConditionType == CT_RightParen) {
    conditions.push_back(new AndOperator());
  }
  conditions.push_back(cond);
  LastConditionType = CT_Operand;
}

void Condition::BuildConditions(std::vector<Condition*> &conditions, std::string token) {
  std::vector<Condition*> endConditions;
  int i;

  // Since we don't have a real parser, things will break if [!()] appear in
  // the middle of a token (e.g. "(X AND Y)(A AND B)")

  // Look for syntax characters at the beginning of the token
  for (i = 0; i < (int)token.length(); i++) {
    if (token[i] == '!') {
      Condition::AddNonOperand(conditions, new NegationOperator());
    } else if (token[i] == '(') {
      Condition::AddNonOperand(conditions, new LeftParen());
    } else if (token[i] == ')') {
      Condition::AddNonOperand(conditions, new RightParen());
    } else {
      break;
    }
  }
  token.erase(0, i);

  // Look for syntax characters at the end of the token
  for (i = token.length() - 1; i >= 0; i--) {
    if (token[i] == '!') {
      endConditions.insert(endConditions.begin(), new NegationOperator());
    } else if (token[i] == '(') {
      endConditions.insert(endConditions.begin(), new LeftParen());
    } else if (token[i] == ')') {
      endConditions.insert(endConditions.begin(), new RightParen());
    } else {
      break;
    }
  }
  if (i < (int)(token.length() - 1)) {
    token.erase(i + 1, std::string::npos);
  }

  size_t delPos = token.find_first_of(tokenDelims);
  std::string key;
  std::string delim = "";
  int value = 0;
  std::string valueStr;
  if (delPos != std::string::npos) {
    key = Trim(token.substr(0, delPos));
    delim = token.substr(delPos, 1);
    valueStr = Trim(token.substr(delPos + 1));
    if (valueStr.length() > 0) {
      std::stringstream ss(valueStr);
      if ((ss >> value).fail()) {
        std::cout << "Error processing value for token: " << token << std::endl;
        return;  // TODO: returning errors
      }
    }
  } else {
    key = token;
  }
  //if (key.compare(0, 5, "COUNT") == 0) PrintText(1, "Matched COUNT, valueStr=%s, value=%d, delim=%s", valueStr.c_str(), value, delim.c_str());
  BYTE operation = GetOperation(&delim);

  unsigned int keylen = key.length();
  if (key == "AND" || key == "&&") {
    Condition::AddNonOperand(conditions, new AndOperator());
  } else if (key == "OR" || key == "||") {
    Condition::AddNonOperand(conditions, new OrOperator());
  } else if (keylen == 3 && !(isupper(key[0]) || isupper(key[1]) || isupper(key[2]))) {
    Condition::AddOperand(conditions, new ItemCodeCondition(key.substr(0, 3).c_str()));
  } else if (key.find('+') != std::string::npos) {
    Condition::AddOperand(conditions, new AddCondition(key, operation, value));
  } else if (key == "ETH") {
    Condition::AddOperand(conditions, new FlagsCondition(ITEM_ETHEREAL));
  } else if (key == "SOCK") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_SOCKETS, 0, operation, value));
  } else if (key.compare(0, 3, "SET") == 0) {
    std::smatch match;
    if (std::regex_search(key, match, std::regex("^SET([0-9]{1,4})$")) && match.size() == 2) {
      int id = std::stoi(match[1], nullptr, 10);
      Condition::AddOperand(conditions, new QualityIdCondition(ITEM_QUALITY_SET, id));
    } else {
      Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_SET));
    }
  } else if (key == "MAG") {
    Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_MAGIC));
  } else if (key == "RARE") {
    Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_RARE));
  } else if (key.compare(0, 3, "UNI") == 0) {
    std::smatch match;
    if (regex_search(key, match, std::regex("^UNI([0-9]{1,4})$")) && match.size() == 2) {
      int id = stoi(match[1], nullptr, 10);
      Condition::AddOperand(conditions, new QualityIdCondition(ITEM_QUALITY_UNIQUE, id));
    } else {
      Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_UNIQUE));
    }
  } else if (key == "CRAFTALVL") {
    Condition::AddOperand(conditions, new CraftAffixLevelCondition(operation, value));
  } else if (key == "CRAFT") {
    Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_CRAFT));
  } else if (key == "RW") {
    Condition::AddOperand(conditions, new FlagsCondition(ITEM_RUNEWORD));
  } else if (key == "NMAG") {
    Condition::AddOperand(conditions, new NonMagicalCondition());
  } else if (key == "SUP") {
    Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_SUPERIOR));
  } else if (key == "INF") {
    Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_INFERIOR));
  } else if (key == "NORM") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_NORMAL));
  } else if (key == "EXC") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_EXCEPTIONAL));
  } else if (key == "ELT") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ELITE));
  } else if (key == "ID") {
    Condition::AddOperand(conditions, new FlagsCondition(ITEM_IDENTIFIED));
  } else if (key == "ILVL") {
    Condition::AddOperand(conditions, new ItemLevelCondition(operation, value));
  } else if (key == "QLVL") {
    Condition::AddOperand(conditions, new QualityLevelCondition(operation, value));
  } else if (key == "ALVL") {
    Condition::AddOperand(conditions, new AffixLevelCondition(operation, value));
  } else if (key == "CLVL") {
    Condition::AddOperand(conditions, new CharStatCondition(STAT_LEVEL, 0, operation, value));
  } else if (key == "FILTLVL") {
    Condition::AddOperand(conditions, new FilterLevelCondition(operation, value));
  } else if (key == "DIFF") {
    Condition::AddOperand(conditions, new DifficultyCondition(operation, value));
  } else if (key == "RUNE") {
    Condition::AddOperand(conditions, new RuneCondition(operation, value));
  } else if (key == "GOLD") {
    Condition::AddOperand(conditions, new GoldCondition(operation, value));
  } else if (key == "GEMTYPE") {
    Condition::AddOperand(conditions, new GemTypeCondition(operation, value));
  } else if (key == "GEM") {
    Condition::AddOperand(conditions, new GemLevelCondition(operation, value));
  } else if (key == "ED") {
    Condition::AddOperand(conditions, new EDCondition(operation, value));
  } else if (key == "DEF") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_DEFENSE, 0, operation, value));
  } else if (key == "MAXDUR") {
    Condition::AddOperand(conditions, new DurabilityCondition(operation, value));
  } else if (key == "RES") {
    Condition::AddOperand(conditions, new ResistAllCondition(operation, value));
  } else if (key == "FRES") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_FIRERESIST, 0, operation, value));
  } else if (key == "CRES") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_COLDRESIST, 0, operation, value));
  } else if (key == "LRES") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_LIGHTNINGRESIST, 0, operation, value));
  } else if (key == "PRES") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_POISONRESIST, 0, operation, value));
  } else if (key == "IAS") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_IAS, 0, operation, value));
  } else if (key == "FCR") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_FASTERCAST, 0, operation, value));
  } else if (key == "FHR") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_FASTERHITRECOVERY, 0, operation, value));
  } else if (key == "FBR") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_FASTERBLOCK, 0, operation, value));
  } else if (key == "LIFE") {
    // For unknown reasons, the game's internal HP stat is 256 for every 1 displayed on item
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_MAXHP, 0, operation, value * 256));
  } else if (key == "MANA") {
    // For unknown reasons, the game's internal Mana stat is 256 for every 1 displayed on item
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_MAXMANA, 0, operation, value * 256));
  } else if (key == "GOODSK") {
    Condition::AddOperand(conditions, new SkillListCondition(operation, CLASS_SKILLS, value));
  }else if (key == "GOODTBSK") {
    Condition::AddOperand(conditions, new SkillListCondition(operation, CLASS_TAB_SKILLS, value));
  } else if (key == "FOOLS") {
    Condition::AddOperand(conditions, new FoolsCondition());
  } else if (key == "LVLREQ") {
    Condition::AddOperand(conditions, new RequiredLevelCondition(operation, value));
  } else if (key == "ARPER") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_TOHITPERCENT, 0, operation, value));
  } else if (key == "MFIND") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_MAGICFIND, 0, operation, value));
  } else if (key == "GFIND") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_GOLDFIND, 0, operation, value));
  } else if (key == "STR") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_STRENGTH, 0, operation, value));
  } else if (key == "DEX") {
    //PrintText(1, "In BuildCondition. Creating DEX condition with value=%d", value);
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_DEXTERITY, 0, operation, value));
  } else if (key == "FRW") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_FASTERRUNWALK, 0, operation, value));
  } else if (key == "MINDMG") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_MINIMUMDAMAGE, 0, operation, value));
  } else if (key == "MAXDMG") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_MAXIMUMDAMAGE, 0, operation, value));
  } else if (key == "AR" && keylen == 2) {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_ATTACKRATING, 0, operation, value));
  } else if (key == "DTM") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_DAMAGETOMANA, 0, operation, value));
  } else if (key == "MAEK") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_MANAAFTEREACHKILL, 0, operation, value));
  } else if (key == "REPLIFE") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_REPLENISHLIFE, 0, operation, value));
  } else if (key == "REPQUANT") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_REPLENISHESQUANTITY, 0, operation, value));
  } else if (key == "REPAIR") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_REPAIRSDURABILITY, 0, operation, value));
  } else if (key == "ARMOR") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ALLARMOR));
  } else if (key == "BELT") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BELT));
  } else if (key == "CHEST") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ARMOR));
  } else if (key == "HELM") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_HELM));
  } else if (key == "SHIELD") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SHIELD));
  } else if (key == "GLOVES") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_GLOVES));
  } else if (key == "BOOTS") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BOOTS));
  } else if (key == "CIRC") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_CIRCLET));
  } else if (key == "DRU") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_DRUID_PELT));
  } else if (key == "BAR") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BARBARIAN_HELM));
  } else if (key == "DIN") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_PALADIN_SHIELD));
  } else if (key == "NEC") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_NECROMANCER_SHIELD));
  } else if (key == "SIN") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ASSASSIN_KATAR));
  } else if (key == "SOR") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SORCERESS_ORB));
  } else if (key == "ZON") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_AMAZON_WEAPON));
  } else if (key == "AXE") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_AXE));
  } else if (key == "MACE") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_MACE));
  } else if (key == "SWORD") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SWORD));
  } else if (key == "DAGGER") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_DAGGER));
  } else if (key == "THROWING") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_THROWING));
  } else if (key == "JAV") {
    // Javelins don't seem to have ITEM_GROUP_JAVELIN set
    // they are however, throwing spears
    Condition::AddOperand(conditions,
      new ItemGroupCondition(ITEM_GROUP_THROWING | ITEM_GROUP_SPEAR));
  } else if (key == "SPEAR") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SPEAR));
  } else if (key == "POLEARM") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_POLEARM));
  } else if (key == "BOW") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BOW));
  } else if (key == "XBOW") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_CROSSBOW));
  } else if (key == "STAFF") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_STAFF));
  } else if (key == "WAND") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_WAND));
  } else if (key == "SCEPTER") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SCEPTER));
  } else if (key.compare(0, 2, "EQ") == 0 && keylen == 3) {
    if (key[2] == '1') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_HELM));
    } else if (key[2] == '2') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ARMOR));
    } else if (key[2] == '3') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SHIELD));
    } else if (key[2] == '4') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_GLOVES));
    } else if (key[2] == '5') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BOOTS));
    } else if (key[2] == '6') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BELT));
    } else if (key[2] == '7') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_CIRCLET));
    }
  } else if (key.compare(0, 2, "CL") == 0 && keylen == 3) {
    if (key[2] == '1') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_DRUID_PELT));
    } else if (key[2] == '2') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BARBARIAN_HELM));
    } else if (key[2] == '3') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_PALADIN_SHIELD));
    } else if (key[2] == '4') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_NECROMANCER_SHIELD));
    } else if (key[2] == '5') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ASSASSIN_KATAR));
    } else if (key[2] == '6') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SORCERESS_ORB));
    } else if (key[2] == '7') {
      Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_AMAZON_WEAPON));
    }
  } else if (key == "WEAPON") {
    Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ALLWEAPON));
  } else if (key.compare(0, 2, "WP") == 0) {
    if (keylen >= 3) {
      if (key[2] == '1') {
        if (keylen >= 4 && key[3] == '0') {
          Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_CROSSBOW));
        } else if (keylen >= 4 && key[3] == '1') {
          Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_STAFF));
        } else if (keylen >= 4 && key[3] == '2') {
          Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_WAND));
        } else if (keylen >= 4 && key[3] == '3') {
          Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SCEPTER));
        } else {
          Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_AXE));
        }
      } else if (key[2] == '2') {
        Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_MACE));
      } else if (key[2] == '3') {
        Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SWORD));
      } else if (key[2] == '4') {
        Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_DAGGER));
      } else if (key[2] == '5') {
        Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_THROWING));
      } else if (key[2] == '6') {
        // Javelins don't seem to have ITEM_GROUP_JAVELIN set
        // they are however, throwing spears
        Condition::AddOperand(conditions,
          new ItemGroupCondition(ITEM_GROUP_THROWING | ITEM_GROUP_SPEAR));
      } else if (key[2] == '7') {
        Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SPEAR));
      } else if (key[2] == '8') {
        Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_POLEARM));
      } else if (key[2] == '9') {
        Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BOW));
      }
    }
  } else if (key.compare(0, 2, "SK") == 0) {
    int num = -1;
    std::stringstream ss(key.substr(2));
    if ((ss >> num).fail() || num < 0 || num > (int)SKILL_MAX) {
      return;
    }
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_SINGLESKILL, num, operation, value));
  } else if (key.compare(0, 2, "OS") == 0) {
    int num = -1;
    std::stringstream ss(key.substr(2));
    if ((ss >> num).fail() || num < 0 || num > (int)SKILL_MAX) {
      return;
    }
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_NONCLASSSKILL, num, operation, value));
  } else if (key.compare(0, 4, "CHSK") == 0) { // skills granted by charges
    int num = -1;
    std::stringstream ss(key.substr(4));
    if ((ss >> num).fail() || num < 0 || num > (int)SKILL_MAX) {
      return;
    }
    Condition::AddOperand(conditions, new ChargedCondition(operation, num, value));
  } else if (key.compare(0, 4, "CLSK") == 0) {
    int num = -1;
    std::stringstream ss(key.substr(4));
    if ((ss >> num).fail() || num < 0 || num >= CLASS_NA) {
      return;
    }
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_CLASSSKILLS, num, operation, value));
  } else if (key == "ALLSK") {
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_ALLSKILLS, 0, operation, value));
  } else if (key.compare(0, 5, "TABSK") == 0) {
    int num = -1;
    std::stringstream ss(key.substr(5));
    if ((ss >> num).fail() || num < 0 || num > SKILLTAB_MAX) {
      return;
    }
    Condition::AddOperand(conditions, new ItemStatCondition(STAT_SKILLTAB, num, operation, value));
  } else if (key.compare(0, 4, "STAT") == 0) {
    int num = -1;
    std::stringstream ss(key.substr(4));
    if ((ss >> num).fail() || num < 0 || num > (int)STAT_MAX) {
      return;
    }
    Condition::AddOperand(conditions, new ItemStatCondition(num, 0, operation, value));
  } else if (key.compare(0, 8, "CHARSTAT") == 0) {
    int num = -1;
    std::stringstream ss(key.substr(8));
    if ((ss >> num).fail() || num < 0 || num > (int)STAT_MAX) {
      return;
    }
    Condition::AddOperand(conditions, new CharStatCondition(num, 0, operation, value));
  } else if (key.compare(0, 5, "MULTI") == 0) {

    std::regex multi_reg("([0-9]{1,10}),([0-9]{1,10})",
      std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::smatch multi_match;
    if (std::regex_search(key, multi_match, multi_reg)) {
      int stat1, stat2;
      stat1 = stoi(multi_match[1].str(), nullptr, 10);
      stat2 = stoi(multi_match[2].str(), nullptr, 10);

      Condition::AddOperand(conditions, new ItemStatCondition(stat1, stat2, operation, value));
    }

  } else if (key == "PRICE") {
    Condition::AddOperand(conditions, new ItemPriceCondition(operation, value));
  } else if (key == "XP") {
    Condition::AddOperand(conditions, new PlayerTypeCondition(PLAYER_XP));
  } else if (key == "CLASSIC") {
  Condition::AddOperand(conditions, new PlayerTypeCondition(PLAYER_CLASSIC));
  } else if (key.compare(0, 5, "COUNT") == 0) {
    // backup the last condition type
    //PrintText(1, "COUNT match with valueStr=%s", valueStr.c_str());
    int i = 0; // Token index
    std::string s(valueStr);
    const std::string delimiter = ","; // Partial conditions are delimited by commas, e.g., COUNT=2,FRES>30,LRES>30,CRES>30
    size_t last = 0; 
    size_t next = 0;
    int min_conditions = 0; // minimum number of conditions required to match
    std::vector<Rule> rule_vec;
    std::vector<std::string> tokens;
    while ((next = s.find(delimiter, last)) != std::string::npos) {
      if (i==0) {
        std::stringstream ss(s.substr(last, next-last));
        if ((ss >> min_conditions).fail()) {
          // TODO: Error handling
          return;
        }
        if (min_conditions != value) return; // TODO: Error handling
      } else {
        tokens.push_back(s.substr(last, next-last));
      }
      last = next + 1;
      i++;
    }
    tokens.push_back(s.substr(last));
    // substitue | for a space. this is a workaround since we can't allow spaces in a single token
    for (auto &token : tokens) {
      replace(token.begin(), token.end(), '|', ' ');
    }
    //PrintText(1, "Created PartialCondition with min_conditions=%d and rules size=%d", min_conditions, tokens.size());
    Condition::AddOperand(conditions, new PartialCondition(operation, min_conditions, tokens));
  } else if ( token.length() > 0 ){
    PrintText(1, "Ignored ItemDisplay token: %s", token.c_str());
    std::cout << "Ignored ItemDisplay token: " << token << std::endl;
  }
  for (std::vector<Condition*>::iterator it = endConditions.begin(); it != endConditions.end(); it++) {
    Condition::AddNonOperand(conditions, (*it));
  }
}

void Condition::AddNonOperand(std::vector<Condition*> &conditions, Condition *cond) {
  if ((cond->conditionType == CT_NegationOperator || cond->conditionType == CT_LeftParen) &&
    (LastConditionType == CT_Operand || LastConditionType == CT_RightParen)) {
    conditions.push_back(new AndOperator());
  }
  conditions.push_back(cond);
  LastConditionType = cond->conditionType;
}

bool Condition::Evaluate(UnitItemInfo *uInfo, ItemInfo *info, Condition *arg1, Condition *arg2) {
  // Arguments will vary based on where we're called from.
  // We will have either *info set (if called on reception of packet 0c9c, in which case
  // the normal item structures won't have been set up yet), or *uInfo otherwise.
  if (info) {
    return EvaluateInternalFromPacket(info, arg1, arg2);
  }
  return EvaluateInternal(uInfo, arg1, arg2);
}

ItemLevelCondition::ItemLevelCondition(BYTE op, BYTE ilvl) :
    itemLevel(ilvl), operation(op) {
  conditionType = CT_Operand;
};

// make_count_subrule calls BuildConditon, which creates new Conditions. We free these here.
PartialCondition::~PartialCondition() {
  for (auto rule : rules) {
    for (Condition *condition : rule.conditions) {
      delete condition;
    }
  }
}

void PartialCondition::make_count_subrule(std::string rule_str) {
  BYTE LastConditionTypeOld = LastConditionType;
  LastConditionType = CT_None;
  std::vector<Condition*> RawConditions;
  std::string buf;
  std::vector<std::string> tokens;
  std::stringstream ss(rule_str);
  while (ss >> buf) {
    tokens.push_back(buf);
  }
  for (auto &token : tokens) {
    //std::string token(s.substr(last, next-last));
    //PrintText(1, "In BuildConditions. token=%s", token.c_str());
    Condition::BuildConditions(RawConditions, token);
    //PrintText(1, "In BuildConditions. RawConditions.size=%d", RawConditions.size());
  }
  for (auto condition : RawConditions) {
    //PrintText(1, "\t condition type=%d", condition->conditionType);
  }
  Rule rule(RawConditions, NULL);
  rules.push_back(rule);
  LastConditionType = LastConditionTypeOld;
}


bool TrueCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return true;
}
bool TrueCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return true;
}

bool FalseCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return false;
}
bool FalseCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return false;
}

bool NegationOperator::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return !arg1->Evaluate(uInfo, NULL, arg1, arg2);
}
bool NegationOperator::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return !arg1->Evaluate(NULL, info, arg1, arg2);
}

bool LeftParen::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return false;
}
bool LeftParen::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return false;
}

bool RightParen::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return false;
}
bool RightParen::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return false;
}

bool AndOperator::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return arg1->Evaluate(uInfo, NULL, NULL, NULL) && arg2->Evaluate(uInfo, NULL, NULL, NULL);
}
bool AndOperator::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return arg1->Evaluate(NULL, info, NULL, NULL) && arg2->Evaluate(NULL, info, NULL, NULL);
}

bool OrOperator::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return arg1->Evaluate(uInfo, NULL, NULL, NULL) || arg2->Evaluate(uInfo, NULL, NULL, NULL);
}
bool OrOperator::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return arg1->Evaluate(NULL, info, NULL, NULL) || arg2->Evaluate(NULL, info, NULL, NULL);
}

bool ItemCodeCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return (targetCode[0] == uInfo->itemCode[0] && targetCode[1] == uInfo->itemCode[1] && targetCode[2] == uInfo->itemCode[2]);
}
bool ItemCodeCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return (targetCode[0] == info->code[0] && targetCode[1] == info->code[1] && targetCode[2] == info->code[2]);
}
bool QualityIdCondition::EvaluateInternal(UnitItemInfo* uInfo, Condition* arg1, Condition* arg2) {
  return uInfo->item->pItemData->dwFileIndex == id && uInfo->item->pItemData->dwQuality == quality;
}
bool QualityIdCondition::EvaluateInternalFromPacket(ItemInfo* info, Condition* arg1, Condition* arg2) {
  switch (quality) {
  case ITEM_QUALITY_UNIQUE:
    return info->uniqueCode == id;
  case ITEM_QUALITY_SET:
    return info->setCode == id;
  default:
    return false;
  }
}

bool FlagsCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return ((uInfo->item->pItemData->dwFlags & flag) > 0);
}
bool FlagsCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  switch (flag) {
  case ITEM_ETHEREAL:
    return info->ethereal;
  case ITEM_IDENTIFIED:
    return info->identified;
  case ITEM_RUNEWORD:
    return info->runeword;
  }
  return false;
}

bool PlayerTypeCondition::EvaluateInternal(UnitItemInfo* uInfo, Condition* arg1, Condition* arg2) {
  return (((*p_D2LAUNCH_BnData)->nCharFlags >> 5) & 0x1) == mode;
}
bool PlayerTypeCondition::EvaluateInternalFromPacket(ItemInfo* info, Condition* arg1, Condition* arg2) {
  return (((*p_D2LAUNCH_BnData)->nCharFlags >> 5) & 0x1) == mode;
}

bool QualityCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return (uInfo->item->pItemData->dwQuality == quality);
}
bool QualityCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return (info->quality == quality);
}

bool NonMagicalCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return (uInfo->item->pItemData->dwQuality == ITEM_QUALITY_INFERIOR ||
      uInfo->item->pItemData->dwQuality == ITEM_QUALITY_NORMAL ||
      uInfo->item->pItemData->dwQuality == ITEM_QUALITY_SUPERIOR);
}
bool NonMagicalCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return (info->quality == ITEM_QUALITY_INFERIOR ||
      info->quality == ITEM_QUALITY_NORMAL ||
      info->quality == ITEM_QUALITY_SUPERIOR);
}

bool GemLevelCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  if (IsGem(uInfo->attrs)) {
    return IntegerCompare(GetGemLevel(uInfo->attrs), operation, gemLevel);
  }
  return false;
}
bool GemLevelCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  if (IsGem(info->attrs)) {
    return IntegerCompare(GetGemLevel(info->attrs), operation, gemLevel);
  }
  return false;
}
bool GemTypeCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  if (IsGem(uInfo->attrs)) {
    return IntegerCompare(GetGemType(uInfo->attrs), operation, gemType);
  }
  return false;
}
bool GemTypeCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  if (IsGem(info->attrs)) {
    return IntegerCompare(GetGemType(info->attrs), operation, gemType);
  }
  return false;
}

bool RuneCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  if (IsRune(uInfo->attrs)) {
    return IntegerCompare(RuneNumberFromItemCode(uInfo->itemCode), operation, runeNumber);
  }
  return false;
}
bool RuneCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  if (IsRune(info->attrs)) {
    return IntegerCompare(RuneNumberFromItemCode(info->code), operation, runeNumber);
  }
  return false;
}

bool GoldCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return false; // can only evaluate this from packet data
}
bool GoldCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  if (info->code[0] == 'g' && info->code[1] == 'l' && info->code[2] == 'd') {
    return IntegerCompare(info->amount, operation, goldAmount);
  }
  return false;
}

bool ItemLevelCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return IntegerCompare(uInfo->item->pItemData->dwItemLevel, operation, itemLevel);
}
bool ItemLevelCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return IntegerCompare(info->level, operation, itemLevel);
}

bool QualityLevelCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  BYTE qlvl = uInfo->attrs->qualityLevel;
  return IntegerCompare(qlvl, operation, qualityLevel);
}
bool QualityLevelCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  int qlvl = info->attrs->qualityLevel;
  return IntegerCompare(qlvl, operation, qualityLevel);
}

bool AffixLevelCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  BYTE qlvl = uInfo->attrs->qualityLevel;
  BYTE alvl = GetAffixLevel((BYTE)uInfo->item->pItemData->dwItemLevel, (BYTE)uInfo->attrs->qualityLevel, uInfo->attrs->magicLevel);
  return IntegerCompare(alvl, operation, affixLevel);
}
bool AffixLevelCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  int qlvl = info->attrs->qualityLevel;
  BYTE alvl = GetAffixLevel(info->level, info->attrs->qualityLevel, info->attrs->magicLevel);
  return IntegerCompare(alvl, operation, affixLevel);
}

bool CraftAffixLevelCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  BYTE qlvl = uInfo->attrs->qualityLevel;
  auto ilvl = uInfo->item->pItemData->dwItemLevel;
  auto clvl = D2COMMON_GetUnitStat(D2CLIENT_GetPlayerUnit(), STAT_LEVEL, 0); 
  auto craft_ilvl = ilvl/2 + clvl/2;
  BYTE alvl = GetAffixLevel((BYTE)craft_ilvl, (BYTE)uInfo->attrs->qualityLevel, uInfo->attrs->magicLevel);
  return IntegerCompare(alvl, operation, affixLevel);
}
bool CraftAffixLevelCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  int qlvl = info->attrs->qualityLevel;
  auto ilvl = info->level;
  auto clvl = D2COMMON_GetUnitStat(D2CLIENT_GetPlayerUnit(), STAT_LEVEL, 0); 
  auto craft_ilvl = ilvl/2 + clvl/2;
  BYTE alvl = GetAffixLevel((BYTE)craft_ilvl, info->attrs->qualityLevel, info->attrs->magicLevel);
  return IntegerCompare(alvl, operation, affixLevel);
}

bool RequiredLevelCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  unsigned int rlvl = GetRequiredLevel(uInfo->item);

  return IntegerCompare(rlvl, operation, requiredLevel);
}
bool RequiredLevelCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {

  //Not Done Yet (is it necessary? I think this might have something to do with the exact moment something drops?)

  return true;
}

bool ItemGroupCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return ((uInfo->attrs->flags & itemGroup) > 0);
}
bool ItemGroupCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return ((info->attrs->flags & itemGroup) > 0);
}

bool EDCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  // Either enhanced defense or enhanced damage depending on item type
  WORD stat;
  if (uInfo->attrs->flags & ITEM_GROUP_ALLARMOR) {
    stat = STAT_ENHANCEDDEFENSE;
  } else {
    // Normal %ED will have the same value for STAT_ENHANCEDMAXIMUMDAMAGE and STAT_ENHANCEDMINIMUMDAMAGE
    stat = STAT_ENHANCEDMAXIMUMDAMAGE;
  }

  // Pulled from JSUnit.cpp in d2bs
  DWORD value = 0;
  Stat aStatList[256] = { NULL };
  StatList* pStatList = D2COMMON_GetStatList(uInfo->item, NULL, 0x40);
  if (pStatList) {
    DWORD dwStats = D2COMMON_CopyStatList(pStatList, (Stat*)aStatList, 256);
    for (UINT i = 0; i < dwStats; i++) {
      if (aStatList[i].wStatIndex == stat && aStatList[i].wSubIndex == 0) {
        value += aStatList[i].dwStatValue;
      }
    }
  }
  return IntegerCompare(value, operation, targetED);
}
bool EDCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  // Either enhanced defense or enhanced damage depending on item type
  WORD stat;
  if (info->attrs->flags & ITEM_GROUP_ALLARMOR) {
    stat = STAT_ENHANCEDDEFENSE;
  } else {
    // Normal %ED will have the same value for STAT_ENHANCEDMAXIMUMDAMAGE and STAT_ENHANCEDMINIMUMDAMAGE
    stat = STAT_ENHANCEDMAXIMUMDAMAGE;
  }

  DWORD value = 0;
  for (std::vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
    if (prop->stat == stat) {
      value += prop->value;
    }
  }
  return IntegerCompare(value, operation, targetED);
}

bool DurabilityCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  // Pulled from JSUnit.cpp in d2bs
  DWORD value = 0;
  Stat aStatList[256] = { NULL };
  StatList* pStatList = D2COMMON_GetStatList(uInfo->item, NULL, 0x40);
  if (pStatList) {
    DWORD dwStats = D2COMMON_CopyStatList(pStatList, (Stat*)aStatList, 256);
    for (UINT i = 0; i < dwStats; i++) {
      if (aStatList[i].wStatIndex == STAT_ENHANCEDMAXDURABILITY && aStatList[i].wSubIndex == 0) {
        value += aStatList[i].dwStatValue;
      }
    }
  }
  return IntegerCompare(value, operation, targetDurability);
}
bool DurabilityCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  DWORD value = 0;
  for (std::vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
    if (prop->stat == STAT_ENHANCEDMAXDURABILITY) {
      value += prop->value;
    }
  }
  return IntegerCompare(value, operation, targetDurability);
}

bool ChargedCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  DWORD value = 0;
  Stat aStatList[256] = { NULL };
  StatList* pStatList = D2COMMON_GetStatList(uInfo->item, NULL, 0x40);
  if (pStatList) {
    DWORD dwStats = D2COMMON_CopyStatList(pStatList, (Stat*)aStatList, 256);
    for (UINT i = 0; i < dwStats; i++) {
      //if (aStatList[i].wStatIndex == STAT_CHARGED)
      //	PrintText(1, "ChargedCondition::EvaluateInternal: Index=%hx, SubIndex=%hx, Value=%x", aStatList[i].wStatIndex, aStatList[i].wSubIndex, aStatList[i].dwStatValue);
      if (aStatList[i].wStatIndex == STAT_CHARGED && (aStatList[i].wSubIndex>>6) == skill) { // 10 MSBs of subindex is the skill ID
        unsigned int level = aStatList[i].wSubIndex & 0x3F; // 6 LSBs are the skill level
        value = (level > value) ? level : value; // use highest level
      }
    }
  }
  return IntegerCompare(value, operation, targetLevel);
}
bool ChargedCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  DWORD num = 0;
  for (std::vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
    if (prop->stat == STAT_CHARGED && prop->skill == skill) {
      num = (prop->level > num) ? prop->level : num; // use the highest level charges for the comparison
      //PrintText(1, "Found charged skill. skill=%u level=%u", prop->skill, prop->level);
    }
  }
  return IntegerCompare(num, operation, targetLevel);
}

bool FoolsCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  // 1 = MAX DMG / level
  // 2 = AR / level
  // 3 = Fools

  // Pulled from JSUnit.cpp in d2bs
  unsigned int value = 0;
  Stat aStatList[256] = { NULL };
  StatList* pStatList = D2COMMON_GetStatList(uInfo->item, NULL, 0x40);
  if (pStatList) {
    DWORD dwStats = D2COMMON_CopyStatList(pStatList, (Stat*)aStatList, 256);
    for (UINT i = 0; i < dwStats; i++) {
      if (aStatList[i].wStatIndex == STAT_MAXDAMAGEPERLEVEL && aStatList[i].wSubIndex == 0) {
        value += 1;
      }
      if (aStatList[i].wStatIndex == STAT_ATTACKRATINGPERLEVEL && aStatList[i].wSubIndex == 0) {
        value += 2;
      }
    }
  }
  // We are returning a comparison on 3 here instead of any the actual number because the way it is setup is
  // to just write FOOLS in the mh file instead of FOOLS=3 this could be changed to accept 1-3 for the different
  // types it can produce
  return IntegerCompare(value, (BYTE)EQUAL, 3);
}
bool FoolsCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  // 1 = MAX DMG / level
  // 2 = AR / level
  // 3 = Fools

  unsigned int value = 0;
  for (std::vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
    if (prop->stat == STAT_MAXDAMAGEPERLEVEL) {
      value += 1;
    }
    if (prop->stat == STAT_ATTACKRATINGPERLEVEL) {
      value += 2;
    }
  }
  // We are returning a comparison on 3 here instead of any the actual number because the way it is setup is
  // to just write FOOLS in the mh file instead of FOOLS=3 this could be changed to accept 1-3 for the different
  // types it can produce
  return IntegerCompare(value, (BYTE)EQUAL, 3);
}

void SkillListCondition::Init() {
  // Clear lists
  classSkillList.clear();
  skillList.clear();
  goodClassSkills.clear();
  goodTabSkills.clear();

  // Build character skills list
  BH::itemConfig->ReadAssoc("ClassSkillsList", skillList);
  for (auto it = skillList.cbegin(); it != skillList.cend(); it++) {
    if (ToBool((*it).second)) {
      goodClassSkills.push_back(stoi((*it).first));
    }
  }

  // Build tab skills list
  BH::itemConfig->ReadAssoc("TabSkillsList", classSkillList);
  for (auto it = classSkillList.cbegin(); it != classSkillList.cend(); it++) {
    if (ToBool((*it).second)) {
      goodTabSkills.push_back(stoi((*it).first));
    }
  }
}

bool SkillListCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  int value = 0;
  if (type == CLASS_SKILLS) {
    for (unsigned int i = 0; i < goodClassSkills.size(); i++) {
      value += D2COMMON_GetUnitStat(uInfo->item, STAT_CLASSSKILLS, goodClassSkills.at(i));
    }
  }
  else if (type == CLASS_TAB_SKILLS) {
    for (unsigned int i = 0; i < goodTabSkills.size(); i++) {
      value += D2COMMON_GetUnitStat(uInfo->item, STAT_SKILLTAB, goodTabSkills.at(i));
    }
  }

  return IntegerCompare(value, operation, targetStat);
}

bool SkillListCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  // TODO: Implement later
  return false;
}

bool CharStatCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return IntegerCompare(D2COMMON_GetUnitStat(D2CLIENT_GetPlayerUnit(), stat1, stat2), operation, targetStat);
}
bool CharStatCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return IntegerCompare(D2COMMON_GetUnitStat(D2CLIENT_GetPlayerUnit(), stat1, stat2), operation, targetStat);
}

bool DifficultyCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return IntegerCompare(D2CLIENT_GetDifficulty(), operation, targetDiff);
}
bool DifficultyCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return IntegerCompare(D2CLIENT_GetDifficulty(), operation, targetDiff);
}

bool FilterLevelCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return IntegerCompare(Item::GetFilterLevel(), operation, filterLevel);
}
bool FilterLevelCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  return IntegerCompare(Item::GetFilterLevel(), operation, filterLevel);
}

bool ItemStatCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return IntegerCompare(D2COMMON_GetUnitStat(uInfo->item, itemStat, itemStat2), operation, targetStat);
}
bool ItemStatCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  int num = 0;
  switch (itemStat) {
  case STAT_SOCKETS:
    return IntegerCompare(info->sockets, operation, targetStat);
  case STAT_DEFENSE:
    return IntegerCompare(GetDefense(info), operation, targetStat);
  case STAT_NONCLASSSKILL:
    for (std::vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
      if (prop->stat == STAT_NONCLASSSKILL && prop->skill == itemStat2) {
        num += prop->value;
      }
    }
    return IntegerCompare(num, operation, targetStat);
  case STAT_SINGLESKILL:
    for (std::vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
      if (prop->stat == STAT_SINGLESKILL && prop->skill == itemStat2) {
        num += prop->value;
      }
    }
    return IntegerCompare(num, operation, targetStat);
  case STAT_CLASSSKILLS:
    for (std::vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
      if (prop->stat == STAT_CLASSSKILLS && prop->characterClass == itemStat2) {
        num += prop->value;
      }
    }
    return IntegerCompare(num, operation, targetStat);
  case STAT_SKILLTAB:
    for (std::vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
      if (prop->stat == STAT_SKILLTAB && (prop->characterClass * 8 + prop->tab) == itemStat2) {
        num += prop->value;
      }
    }
    return IntegerCompare(num, operation, targetStat);
  default:
    for (std::vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
      if (prop->stat == itemStat) {
        num += prop->value;
      }
    }
    return IntegerCompare(num, operation, targetStat);
  }
  return false;
}

bool PartialCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  int match_count = 0;
  for (auto &rule : rules) {
    if (rule.Evaluate(uInfo, NULL)) match_count++;
    //PrintText(1, "in EvaluateInternal. rule.conditions.size=%d match_count=%d", rule.conditions.size(), match_count);
  }
  return IntegerCompare(match_count, operation, target_count);
}

bool PartialCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  int match_count = 0;
  for (auto &rule : rules) {
    if (rule.Evaluate(NULL, info)) match_count++;
  }
  return IntegerCompare(match_count, operation, target_count);
}

bool ItemPriceCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  return IntegerCompare(D2COMMON_GetItemPrice(D2CLIENT_GetPlayerUnit(), uInfo->item, D2CLIENT_GetDifficulty(), (DWORD)D2CLIENT_GetQuestInfo(), 0x201, 1), operation, targetStat);
}
bool ItemPriceCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  // TODO: Implement later
  return false;
}

bool ResistAllCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  int fRes = D2COMMON_GetUnitStat(uInfo->item, STAT_FIRERESIST, 0);
  int lRes = D2COMMON_GetUnitStat(uInfo->item, STAT_LIGHTNINGRESIST, 0);
  int cRes = D2COMMON_GetUnitStat(uInfo->item, STAT_COLDRESIST, 0);
  int pRes = D2COMMON_GetUnitStat(uInfo->item, STAT_POISONRESIST, 0);
  return (IntegerCompare(fRes, operation, targetStat) &&
      IntegerCompare(lRes, operation, targetStat) &&
      IntegerCompare(cRes, operation, targetStat) &&
      IntegerCompare(pRes, operation, targetStat));
}
bool ResistAllCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  int fRes = 0, lRes = 0, cRes = 0, pRes = 0;
  for (std::vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
    if (prop->stat == STAT_FIRERESIST) {
      fRes += prop->value;
    } else if (prop->stat == STAT_LIGHTNINGRESIST) {
      lRes += prop->value;
    } else if (prop->stat == STAT_COLDRESIST) {
      cRes += prop->value;
    } else if (prop->stat == STAT_POISONRESIST) {
      pRes += prop->value;
    }
  }
  return (IntegerCompare(fRes, operation, targetStat) &&
      IntegerCompare(lRes, operation, targetStat) &&
      IntegerCompare(cRes, operation, targetStat) &&
      IntegerCompare(pRes, operation, targetStat));
}

void AddCondition::Init() {
  codes.clear();
  codes = split(key, '+');
  for (auto code : codes) {
    for (int j = 0; j < sizeof(skills) / sizeof(skills[0]); j++) {
      if (code == skills[j].key)
        stats.push_back(skills[j].value);
    }
  }
}

bool AddCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
  int value = 0;
  for (unsigned int i = 0; i < stats.size(); i++) {
    int tmpVal = D2COMMON_GetUnitStat(uInfo->item, stats[i], 0);
    if (stats[i] == STAT_MAXHP || stats[i] == STAT_MAXMANA)
      tmpVal /= 256;
    value += tmpVal;
  }

  return IntegerCompare(value, operation, targetStat);
}

bool AddCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
  // TODO: Implement later
  return false;
}

}  // namespace bh::modules::item