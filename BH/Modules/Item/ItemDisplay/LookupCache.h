
#ifndef BH_MODULES_ITEM_ITEM_DISPLAY_LOOKUP_CACHE_H_
#define BH_MODULES_ITEM_ITEM_DISPLAY_LOOKUP_CACHE_H_

#include <string>
#include <vector>

#include "../../../RuleLookupCache.h"
#include "Action.h"
#include "Rule.h"
#include "UnitItemInfo.h"

namespace bh::modules::item {

class ItemDescLookupCache : public RuleLookupCache<std::string> {
  std::string make_cached_T(UnitItemInfo *uInfo) override;
  std::string to_str(const std::string &name) override;

    public:
    ItemDescLookupCache(const std::vector<Rule*> &RuleList) :
      RuleLookupCache<std::string>(RuleList) {}
};

class ItemNameLookupCache : public RuleLookupCache<std::string, const std::string &> {
  std::string make_cached_T(UnitItemInfo *uInfo, const std::string &name) override;
  std::string to_str(const std::string &name) override;

    public:
    ItemNameLookupCache(const std::vector<Rule*> &RuleList) :
      RuleLookupCache<std::string, const std::string&>(RuleList) {}
};

class MapActionLookupCache : public RuleLookupCache<std::vector<Action>> {
  std::vector<Action> make_cached_T(UnitItemInfo *uInfo) override;
  std::string to_str(const std::vector<Action> &actions);

    public:
    MapActionLookupCache(const std::vector<Rule*> &RuleList) :
      RuleLookupCache<std::vector<Action>>(RuleList) {}
};

class IgnoreLookupCache : public RuleLookupCache<bool> {
  bool make_cached_T(UnitItemInfo *uInfo) override;
  std::string to_str(const bool &ignore);

    public:
    IgnoreLookupCache(const std::vector<Rule*> &RuleList) :
      RuleLookupCache<bool>(RuleList) {}
};

extern ItemDescLookupCache item_desc_cache;
extern ItemNameLookupCache item_name_cache;
extern MapActionLookupCache map_action_cache;
extern IgnoreLookupCache do_not_block_cache;
extern IgnoreLookupCache ignore_cache;

}  // namespace bh::modules::item

#endif  // BH_MODULES_ITEM_ITEM_DISPLAY_LOOKUP_CACHE_H_
