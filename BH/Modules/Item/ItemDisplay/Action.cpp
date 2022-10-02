
#include "Action.h"

#include <string>

#include "ColorConstants.h"

namespace bh::modules::item {

Action::Action() :
    colorOnMap(UNDEFINED_COLOR),
    borderColor(UNDEFINED_COLOR),
    dotColor(UNDEFINED_COLOR),
    pxColor(UNDEFINED_COLOR),
    lineColor(UNDEFINED_COLOR),
    notifyColor(UNDEFINED_COLOR),
    pingLevel(0),
    stopProcessing(true),
    noTracking(false),
    name(""),
    description("") {}

}  // namespace bh::modules::item
