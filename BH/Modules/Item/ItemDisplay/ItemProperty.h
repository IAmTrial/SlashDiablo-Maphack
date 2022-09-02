
#ifndef BH_MODULES_ITEM_ITEM_DISPLAY_ITEM_PROPERTY_H_
#define BH_MODULES_ITEM_ITEM_DISPLAY_ITEM_PROPERTY_H_

namespace bh::modules::item {

// Properties that can appear on an item from incoming packets
struct ItemProperty {
  unsigned int stat;
  long value;

  unsigned int minimum;
  unsigned int maximum;
  unsigned int length;

  unsigned int level;
  unsigned int characterClass;
  unsigned int skill;
  unsigned int tab;

  unsigned int monster;

  unsigned int charges;
  unsigned int maximumCharges;

  unsigned int skillChance;

  unsigned int perLevel;
};

}  // namespace bh::modules::item

#endif  // BH_MODULES_ITEM_ITEM_DISPLAY_ITEM_PROPERTY_H_
