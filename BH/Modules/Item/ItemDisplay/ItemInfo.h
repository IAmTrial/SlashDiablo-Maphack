
#ifndef BH_MODULES_ITEM_ITEM_DISPLAY_ITEM_INFO_H_
#define BH_MODULES_ITEM_ITEM_DISPLAY_ITEM_INFO_H_

#include <windows.h>

#include <string>
#include <vector>

#include "../../../MPQInit.h"
#include "ItemProperty.h"

namespace bh::modules::item {

// Item data obtained from an incoming 0x9c packet
struct ItemInfo {
  ItemAttributes *attrs;
  char code[4];
  //std::string packet;
  std::string name;
  std::string earName;
  std::string personalizedName;
  unsigned int id;
  unsigned int x;
  unsigned int y;
  unsigned int amount;
  unsigned int prefix;
  unsigned int suffix;
  unsigned int setCode;
  unsigned int uniqueCode;
  unsigned int runewordId;
  unsigned int defense;
  unsigned int action;
  unsigned int category;
  unsigned int version;
  unsigned int directory;
  unsigned int container;
  unsigned int earLevel;
  unsigned int width;
  unsigned int height;
  unsigned int quality;
  unsigned int graphic;
  unsigned int color;
  unsigned int superiority;
  unsigned int runewordParameter;
  unsigned int maxDurability;
  unsigned int durability;
  BYTE usedSockets;
  BYTE level;
  BYTE earClass;
  BYTE sockets;
  bool equipped;
  bool inSocket;
  bool identified;
  bool switchedIn;
  bool switchedOut;
  bool broken;
  bool potion;
  bool hasSockets;
  bool inStore;
  bool notInSocket;
  bool ear;
  bool startItem;
  bool simpleItem;
  bool ethereal;
  bool personalized;
  bool gambling;
  bool runeword;
  bool ground;
  bool unspecifiedDirectory;
  bool isGold;
  bool hasGraphic;
  bool hasColor;
  bool isArmor;
  bool isWeapon;
  bool indestructible;
  std::vector<unsigned long> prefixes;
  std::vector<unsigned long> suffixes;
  std::vector<ItemProperty> properties;
  bool operator<(ItemInfo const & other) const;
};

}  // namespace bh::modules::item

#endif  // BH_MODULES_ITEM_ITEM_DISPLAY_ITEM_INFO_H_
