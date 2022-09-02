
#ifndef BH_MODULES_ITEM_ITEM_DISPLAY_GEM_RUNE_H_
#define BH_MODULES_ITEM_ITEM_DISPLAY_GEM_RUNE_H_

#include <windows.h>

#include "../../../MPQInit.h"

namespace bh::modules::item {

bool IsGem(const ItemAttributes* attrs);
BYTE GetGemLevel(const ItemAttributes* attrs);
const char* GetGemLevelString(BYTE level);
BYTE GetGemType(const ItemAttributes* attrs);
const char* GetGemTypeString(BYTE type);
bool IsRune(const ItemAttributes* attrs);
BYTE RuneNumberFromItemCode(const char* code);

}  // namespace bh::modules::item

#endif  // BH_MODULES_ITEM_ITEM_DISPLAY_GEM_RUNE_H_