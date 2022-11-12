#pragma once

#ifndef _D2VERSION_H
#define _D2VERSION_H

#include <string>

enum VersionID {
    INVALID = -1,
    VERSION_113c = 0,
    VERSION_113d
};

namespace D2Version {
    [[deprecated("Use bh::d2::game::exe::version::GetRunning")]]
    VersionID GetGameVersionID();

    [[deprecated("Use bh::d2::game::exe::version::GetDisplayName")]]
    std::string GetHumanReadableVersion();
};

#endif
