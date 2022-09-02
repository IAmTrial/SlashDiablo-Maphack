
#include "GemRune.h"

#include "../../../Constants.h"
#include "../../../MPQInit.h"

namespace bh::modules::item {
namespace {

const char* const GemLevels[] = {
  "NONE",
  "Chipped",
  "Flawed",
  "Normal",
  "Flawless",
  "Perfect"
};

const char* const GemTypes[] = {
  "NONE",
  "Amethyst",
  "Diamond",
  "Emerald",
  "Ruby",
  "Sapphire",
  "Topaz",
  "Skull"
};

}  // namespace

bool IsGem(const ItemAttributes* attrs) {
  return (attrs->flags2 & ITEM_GROUP_GEM) > 0;
}

BYTE GetGemLevel(const ItemAttributes* attrs) {
  if (attrs->flags2 & ITEM_GROUP_CHIPPED) {
    return 1;
  } else if (attrs->flags2 & ITEM_GROUP_FLAWED) {
    return 2;
  } else if (attrs->flags2 & ITEM_GROUP_REGULAR) {
    return 3;
  } else if (attrs->flags2 & ITEM_GROUP_FLAWLESS) {
    return 4;
  } else if (attrs->flags2 & ITEM_GROUP_PERFECT) {
    return 5;
  }
  return 0;
}

const char* GetGemLevelString(BYTE level) {
  return GemLevels[level];
}

BYTE GetGemType(const ItemAttributes* attrs) {
  if (attrs->flags2 & ITEM_GROUP_AMETHYST) {
    return 1;
  } else if (attrs->flags2 & ITEM_GROUP_DIAMOND) {
    return 2;
  } else if (attrs->flags2 & ITEM_GROUP_EMERALD) {
    return 3;
  } else if (attrs->flags2 & ITEM_GROUP_RUBY) {
    return 4;
  } else if (attrs->flags2 & ITEM_GROUP_SAPPHIRE) {
    return 5;
  } else if (attrs->flags2 & ITEM_GROUP_TOPAZ) {
    return 6;
  } else if (attrs->flags2 & ITEM_GROUP_SKULL) {
    return 7;
  }
  return 0;
}

const char* GetGemTypeString(BYTE type) {
  return GemTypes[type];
}

bool IsRune(const ItemAttributes* attrs) {
  return (attrs->flags2 & ITEM_GROUP_RUNE) > 0;
}

BYTE RuneNumberFromItemCode(const char* code){
  return (BYTE)(((code[1] - '0') * 10) + code[2] - '0');
}

}  // namespace bh::modules::item
