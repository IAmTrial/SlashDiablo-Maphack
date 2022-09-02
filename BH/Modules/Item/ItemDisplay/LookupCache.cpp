
#include "LookupCache.h"

#include <string>
#include <vector>

#include "../../../RuleLookupCache.h"
#include "Action.h"
#include "ColorConstants.h"
#include "ItemHelper.h"
#include "Rule.h"
#include "UnitItemInfo.h"

namespace bh::modules::item {

// least recently used cache for storing a limited number of item names
ItemDescLookupCache item_desc_cache(DescRuleList);
ItemNameLookupCache item_name_cache(NameRuleList);
MapActionLookupCache map_action_cache(MapRuleList);
IgnoreLookupCache do_not_block_cache(DoNotBlockRuleList);
IgnoreLookupCache ignore_cache(IgnoreRuleList);

// Find the item description. This code is called only when there's a cache miss
std::string ItemDescLookupCache::make_cached_T(UnitItemInfo *uInfo) {
  std::string new_name;
  for (std::vector<Rule*>::const_iterator it = this->RuleList.begin(); it != this->RuleList.end(); it++) {
    if ((*it)->Evaluate(uInfo, NULL)) {
      SubstituteNameVariables(uInfo, new_name, (*it)->action.description);
      if ((*it)->action.stopProcessing) {
        break;
      }
    }
  }
  return new_name;
}

std::string ItemDescLookupCache::to_str(const std::string &name) {
  size_t start_pos = 0;
  std::string itemName(name);
  while ((start_pos = itemName.find('\n', start_pos)) != std::string::npos) {
    itemName.replace(start_pos, 1, " - ");
    start_pos += 3;
  }
  return itemName;
}

// Find the item name. This code is called only when there's a cache miss
std::string ItemNameLookupCache::make_cached_T(UnitItemInfo *uInfo, const std::string &name) {
  std::string new_name(name);
  for (std::vector<Rule*>::const_iterator it = this->RuleList.begin(); it != this->RuleList.end(); it++) {
    if ((*it)->Evaluate(uInfo, NULL)) {
      SubstituteNameVariables(uInfo, new_name, (*it)->action.name);
      if ((*it)->action.stopProcessing) {
        break;
      }
    }
  }
  // if the item is on the ignore list and not the map list, warn the user that this item is normally blocked
  bool blocked = ignore_cache.Get(uInfo);
  std::vector<Action> actions = map_action_cache.Get(uInfo);
  if (blocked) {
    bool has_map_action = false;
    for (auto &action : actions) {
      if (action.colorOnMap != UNDEFINED_COLOR ||
        action.borderColor != UNDEFINED_COLOR ||
        action.dotColor != UNDEFINED_COLOR ||
        action.pxColor != UNDEFINED_COLOR ||
        action.lineColor != UNDEFINED_COLOR) {
        has_map_action = true;
        break;
      }
          
    }
    bool whitelisted = do_not_block_cache.Get(uInfo);
    if (!has_map_action && !whitelisted) return new_name + " [blocked]";
  }
  return new_name;
}

std::string ItemNameLookupCache::to_str(const std::string &name) {
  size_t start_pos = 0;
  std::string itemName(name);
  while ((start_pos = itemName.find('\n', start_pos)) != std::string::npos) {
    itemName.replace(start_pos, 1, " - ");
    start_pos += 3;
  }
  return itemName;
}

std::vector<Action> MapActionLookupCache::make_cached_T(UnitItemInfo *uInfo) {
  std::vector<Action> actions;
  for (std::vector<Rule*>::const_iterator it = this->RuleList.begin(); it != this->RuleList.end(); it++) {
    if ((*it)->Evaluate(uInfo, NULL)) {
      actions.push_back((*it)->action);
    }
  }
  return actions;
}

std::string MapActionLookupCache::to_str(const std::vector<Action> &actions) {
  std::string name;
  for (auto &action : actions) {
    name += action.name + " ";
  }
  return name;
}

bool IgnoreLookupCache::make_cached_T(UnitItemInfo *uInfo) {
  for (std::vector<Rule*>::const_iterator it = this->RuleList.begin(); it != this->RuleList.end(); it++) {
    if ((*it)->Evaluate(uInfo, NULL)) {
      return true;
    }
  }
  return false;
}

std::string IgnoreLookupCache::to_str(const bool &ignore) {
  return ignore ? "blocked" : "not blocked";
}

}  // namespace bh::modules::item
