#pragma once

#ifndef _D2VERSION_H
#define _D2VERSION_H

#include <string>

#include "bh/d2/exe/version.hpp"

enum VersionID {
    INVALID = -1,
    VERSION_113c = 0,
    VERSION_113d
};

namespace D2Version {
    [[deprecated("Use bh::d2::exe::version::GetRunning")]]
    VersionID GetGameVersionID();

    [[deprecated("Use bh::d2::exe::version::GetDisplayName")]]
    std::string GetHumanReadableVersion();

    ::bh::d2::exe::version::Version ToNewVersionEnum(VersionID oldVersion);
    VersionID ToOldVersionEnum(::bh::d2::exe::version::Version newVersion);
};

#endif
