#pragma once

#include <windows.h>

#include <map>
#include <string>
#include <vector>

#include "../../D2Structs.h"
#include "../../MPQInit.h"
#include "../../RuleLookupCache.h"
#include "ItemDisplay/ItemInfo.h"
#include "ItemDisplay/LookupCache.h"
#include "ItemDisplay/Rule.h"

#define EXCEPTION_INVALID_STAT			1
#define EXCEPTION_INVALID_OPERATION		2
#define EXCEPTION_INVALID_OPERATOR		3
#define EXCEPTION_INVALID_FLAG			4
#define EXCEPTION_INVALID_ITEM_TYPE		5
#define EXCEPTION_INVALID_GOLD_TYPE		6

ItemAttributes ItemAttributeList[];
StatProperties StatPropertiesList[];

extern std::map<std::string, std::string> condition_group;

namespace ItemDisplay {
	void InitializeItemRules();
	void UninitializeItemRules();
	bool UntestedSettingsUsed();
}
