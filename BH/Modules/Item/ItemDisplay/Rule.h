
#ifndef BH_MODULES_ITEM_ITEM_DISPLAY_RULE_H_
#define BH_MODULES_ITEM_ITEM_DISPLAY_RULE_H_

#include <vector>

#include "Action.h"
#include "ItemInfo.h"
#include "UnitItemInfo.h"

namespace bh::modules::item {

class Condition;

struct Rule {
  std::vector<Condition*> conditions;
  Action action;
  std::vector<Condition*> conditionStack;

  Rule(std::vector<Condition*> &inputConditions, std::string* str);

  // Evaluate conditions which are in Reverse Polish Notation
  bool Evaluate(UnitItemInfo *uInfo, ItemInfo *info);
};

extern std::vector<Rule*> RuleList;
extern std::vector<Rule*> NameRuleList;
extern std::vector<Rule*> DescRuleList;
extern std::vector<Rule*> MapRuleList;
extern std::vector<Rule*> DoNotBlockRuleList;
extern std::vector<Rule*> IgnoreRuleList;
extern std::vector<std::pair<std::string, std::string>> rules;

}  // namespace bh::modules::item

#endif  // BH_MODULES_ITEM_ITEM_DISPLAY_RULE_H_
