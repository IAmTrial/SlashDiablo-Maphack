
#include "Rule.h"

#include <vector>

#include "Action.h"
#include "Condition.h"
#include "ItemHelper.h"
#include "ItemInfo.h"
#include "UnitItemInfo.h"

namespace bh::modules::item {
namespace {

static TrueCondition trueCondition;
static FalseCondition falseCondition;

}  // namespace

std::vector<Rule*> RuleList;
std::vector<Rule*> NameRuleList;
std::vector<Rule*> DescRuleList;
std::vector<Rule*> MapRuleList;
std::vector<Rule*> DoNotBlockRuleList;
std::vector<Rule*> IgnoreRuleList;
std::vector<std::pair<std::string, std::string>> rules;

Rule::Rule(std::vector<Condition*> &inputConditions, std::string *str) {
	Condition::ProcessConditions(inputConditions, conditions);
	if (str != NULL) BuildAction(str, &action);
	conditionStack.reserve(conditions.size()); // TODO: too large?
}

bool Rule::Evaluate(UnitItemInfo *uInfo, ItemInfo *info) {
  if (conditions.size() == 0) {
    return true;  // a rule with no conditions always matches
  }

  bool retval;
  try {
    // conditionStack was previously declared on the stack within this function. This caused
    // excessive allocaiton calls and was limiting the speed of the item display (causing
    // frame rate drops with complex item displays while ALT was held down). Moving this vector
    // to the object level, preallocating size, and using the resize(0) method to clear avoids
    // excessive reallocation.
    conditionStack.resize(0);
    for (unsigned int i = 0; i < conditions.size(); i++) {
      Condition *input = conditions[i];
      if (input->conditionType == CT_Operand) {
        conditionStack.push_back(input);
      } else if (input->conditionType == CT_BinaryOperator || input->conditionType == CT_NegationOperator) {
        Condition *arg1 = NULL, *arg2 = NULL;
        if (conditionStack.size() < 1) {
          return false;
        }
        arg1 = conditionStack.back();
        conditionStack.pop_back();
        if (input->conditionType == CT_BinaryOperator) {
          if (conditionStack.size() < 1) {
            return false;
          }
          arg2 = conditionStack.back();
          conditionStack.pop_back();
        }
        if (input->Evaluate(uInfo, info, arg1, arg2)) {
          conditionStack.push_back(&trueCondition);
        } else {
          conditionStack.push_back(&falseCondition);
        }
      }
    }
    if (conditionStack.size() == 1) {
      retval = conditionStack[0]->Evaluate(uInfo, info, NULL, NULL);
    } else {
      retval = false;
    }
  } catch (...) {
    retval = false;
  }
  return retval;
}

}  // namespace bh::modules::item
