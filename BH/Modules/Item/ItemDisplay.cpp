#include "ItemDisplay.h"

#include <stdio.h>
#include <stddef.h>
#include <windows.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../../BH.h"
#include "../../Common.h"
#include "../../Constants.h"
#include "../../D2Ptrs.h"
#include "../../D2Structs.h"
#include "../../MPQInit.h"
#include "../../RuleLookupCache.h"
#include "Item.h"
#include "ItemDisplay/ColorConstants.h"
#include "ItemDisplay/Condition.h"
#include "ItemDisplay/ConditionType.h"
#include "ItemDisplay/GemRune.h"
#include "ItemDisplay/ItemHelper.h"
#include "ItemDisplay/LookupCache.h"
#include "ItemDisplay/Rule.h"

namespace {

using ::bh::modules::item::LastConditionType;
using ::bh::modules::item::item_name_cache;
using ::bh::modules::item::colors;
using ::bh::modules::item::rules;
using ::bh::modules::item::DescRuleList;
using ::bh::modules::item::DoNotBlockRuleList;
using ::bh::modules::item::IgnoreRuleList;
using ::bh::modules::item::MapRuleList;
using ::bh::modules::item::NameRuleList;
using ::bh::modules::item::RuleList;

using ::bh::modules::item::Action;
using ::bh::modules::item::ActionReplace;
using ::bh::modules::item::ColorReplace;
using ::bh::modules::item::Condition;
using ::bh::modules::item::ConditionType;
using ::bh::modules::item::IgnoreLookupCache;
using ::bh::modules::item::ItemDescLookupCache;
using ::bh::modules::item::ItemInfo;
using ::bh::modules::item::ItemProperty;
using ::bh::modules::item::ItemNameLookupCache;
using ::bh::modules::item::GetAffixLevel;
using ::bh::modules::item::GetRequiredLevel;
using ::bh::modules::item::GetGemLevel;
using ::bh::modules::item::GetGemLevelString;
using ::bh::modules::item::GetGemType;
using ::bh::modules::item::GetGemTypeString;
using ::bh::modules::item::IsGem;
using ::bh::modules::item::IsRune;
using ::bh::modules::item::MapActionLookupCache;
using ::bh::modules::item::Rule;
using ::bh::modules::item::RuneNumberFromItemCode;
using ::bh::modules::item::UnitItemInfo;

void removeSubstrs(std::string& s, const std::string& p) {
  std::string::size_type n = p.length();
  for (std::string::size_type i = s.find(p); i != std::string::npos; i = s.find(p))
    s.erase(i, n);
}

std::string without_invis_chars(const std::string &name) {
  std::string wo_invis_chars(name);

  for (int n = 0; n < sizeof(colors) / sizeof(colors[0]); n++) {
    removeSubstrs(wo_invis_chars, "%" + colors[n].key + "%");
  }
  removeSubstrs(wo_invis_chars, " ");
  return wo_invis_chars;
}

}  // namespace

std::map<std::string, std::string> condition_group;

namespace ItemDisplay {
	bool item_display_initialized = false;

	bool UntestedSettingsUsed() {
		return condition_group.size() > 0;
	}

	void InitializeItemRules() {
		if (item_display_initialized) return;
		if (!IsInitialized()){
			return;
		}

		item_display_initialized = true;
		rules.clear();
		ResetCaches();

		condition_group.clear();
		BH::itemConfig->ReadAssoc("ConditionGroup", condition_group);

		BH::itemConfig->ReadMapList("ItemDisplay", rules);
		for (unsigned int i = 0; i < rules.size(); i++) {
			std::string buf;
			std::stringstream ss(rules[i].first);
			std::vector<std::string> tokens;
			while (ss >> buf) {
				// check if buf matches any user idendified strings, and replace it if so
				// todo: make config groups nestable?
				// the group token has to be surrounded by whitespace
				// e.g. `the_group && other_group` works but not `(the_group)`
				if (condition_group.count(buf)) {

					std::string buf2;
					std::stringstream ssg(condition_group[buf]);

					// enclose group with parens
					tokens.push_back("(");

					while (ssg >> buf2) {
						tokens.push_back(buf2);
					}
					tokens.push_back(")");
				}
				else {
					tokens.push_back(buf);
				}
			}

			LastConditionType = ConditionType::CT_None;
			std::vector<Condition*> RawConditions;
			for (std::vector<std::string>::iterator tok = tokens.begin(); tok < tokens.end(); tok++) {
				Condition::BuildConditions(RawConditions, (*tok));
			}
			Rule *r = new Rule(RawConditions, &(rules[i].second));

			RuleList.push_back(r);
			bool has_map_action = false;
			bool has_desc = false;
			bool has_name = false;
			if (without_invis_chars(r->action.description).length() > 0) {
				DescRuleList.push_back(r);
				has_desc = true;
			}
			if (r->action.colorOnMap != UNDEFINED_COLOR ||
					r->action.borderColor != UNDEFINED_COLOR ||
					r->action.dotColor != UNDEFINED_COLOR ||
					r->action.pxColor != UNDEFINED_COLOR ||
					r->action.lineColor != UNDEFINED_COLOR) {
				MapRuleList.push_back(r);
				has_map_action = true;
			}
			if (without_invis_chars(r->action.name).length() > 0) {
				NameRuleList.push_back(r);
				// this is a bit of a hack. the idea is not to block items that have a name specified. Items with a map action are
				// already not blocked, so we make another rule list for those with a name and not a map action. Note the name must
				// not use CONTINUE. If item display line uses continue, then the item can still be blocked by a matching ignore
				// item display line.
				if (r->action.stopProcessing && !has_map_action)
					DoNotBlockRuleList.push_back(r); // if we have a non-blank name and no continue, we don't want to block
				has_name = true;
			}
			if (!has_map_action && !has_name && !has_desc && r->action.stopProcessing) {
				IgnoreRuleList.push_back(r);
			}
		}
		std::cout << "Finished initializing item rules" << std::endl << std::endl;
	}

	void UninitializeItemRules() {
		// RuleList contains every created rule. MapRuleList and IgnoreRuleList have a subset of rules.
		// Deleting objects in RuleList is sufficient.
		if (item_display_initialized) {
			for (Rule *r : RuleList) {
				for (Condition *condition : r->conditions) {
					delete condition;
				}
				delete r;
			}
		}
		item_display_initialized = false;
		ResetCaches();
		RuleList.clear();
		NameRuleList.clear();
		DescRuleList.clear();
		MapRuleList.clear();
		DoNotBlockRuleList.clear();
		IgnoreRuleList.clear();
	}
}
