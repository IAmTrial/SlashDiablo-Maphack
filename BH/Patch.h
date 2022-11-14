#pragma once

#include <windows.h>

#include <string>
#include <vector>

#include "bh/d2/dll/dll.hpp"

enum Dll { D2CLIENT=0,D2COMMON,D2GFX,D2LANG,D2WIN,D2NET,D2GAME,D2LAUNCH,FOG,BNCLIENT, STORM, D2CMP, D2MULTI, D2MCPCLIENT};

::bh::d2::dll::Dll ToNewDllEnum(Dll oldDll);
Dll ToOldDllEnum(::bh::d2::dll::Dll newDll);

enum PatchType { Jump=0xE9, Call=0xE8, NOP=0x90, Push=0x6A };

struct Offsets {
	int _113c;
	int _113d;
};

class Patch {
	private:
		static std::vector<Patch*> Patches;
		Dll dll;
		PatchType type;
		Offsets offsets;
		int length, function;
		BYTE* oldCode;
		bool injected;
	public:
		Patch(PatchType type, Dll dll, Offsets offsets, int function, int length);

		bool Install ();
		bool Remove ();

		bool IsInstalled() { return injected; };

		static int GetDllOffset(Dll dll, int offset);
		static bool WriteBytes(int address, int len, BYTE* bytes);
};
