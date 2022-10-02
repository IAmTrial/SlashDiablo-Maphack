
#ifndef BH_MODULES_ITEM_ITEM_DISPLAY_UNIT_ITEM_INFO_H_
#define BH_MODULES_ITEM_ITEM_DISPLAY_UNIT_ITEM_INFO_H_

#include "../../../D2Structs.h"
#include "../../../MPQInit.h"

namespace bh::modules::item {

// Collection of item data from the internal UnitAny structure
struct UnitItemInfo {
  UnitAny *item;
  char itemCode[4];
  ItemAttributes *attrs;
};

}  // namespace bh::modules::item

#endif  // BH_MODULES_ITEM_ITEM_DISPLAY_UNIT_ITEM_INFO_H_
