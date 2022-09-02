
#ifndef BH_MODULES_ITEM_ITEM_DISPLAY_ACTION_H_
#define BH_MODULES_ITEM_ITEM_DISPLAY_ACTION_H_

#include <string>

namespace bh::modules::item {

struct Action {
  bool stopProcessing;
  std::string name;
  std::string description;
  int colorOnMap;
  int borderColor;
  int dotColor;
  int pxColor;
  int lineColor;
  int notifyColor;
  bool noTracking;
  unsigned int pingLevel;

  Action();
};

}  // namespace bh::modules::item

#endif  // BH_MODULES_ITEM_ITEM_DISPLAY_ACTION_H_
