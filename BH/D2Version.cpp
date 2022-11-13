#include "D2Version.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <windows.h>

#include <string>

#include "bh/common/string_util/wide.hpp"
#include "bh/d2/exe/version.hpp"

namespace {

namespace exe = ::bh::d2::exe;
using NewVersion = exe::version::Version;

using ::bh::common::string_util::wide::ToUtf8;

static VersionID ToOldVersionEnum(NewVersion newVersion) {
	switch (newVersion) {
		case NewVersion::k1_13c: {
			return VersionID::VERSION_113c;
		}

		case NewVersion::k1_13d: {
			return VersionID::VERSION_113d;
		}
	}

	return VersionID::INVALID;
}

}  // namespace

VersionID D2Version::GetGameVersionID() {
	return ToOldVersionEnum(exe::version::GetRunning());
}

std::string D2Version::GetHumanReadableVersion() {
	static std::string utf8DisplayName =
			ToUtf8(
					exe::version::GetDisplayName(
							exe::version::GetRunning()));
	return utf8DisplayName;
}
