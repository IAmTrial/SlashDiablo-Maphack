#include "Patch.h"

#include <string.h>
#include <windows.h>

#include <string>
#include <vector>

#include "D2Version.h"
#include "bh/common/logging/logger.hpp"
#include "bh/d2/dll/dll.hpp"
#include "bh/d2/dll/handle.hpp"
#include "bh/global/file_logger.hpp"

namespace {

using ::bh::common::logging::Logger;
using ::bh::d2::dll::GetHandle;
using ::bh::global::GetFileLogger;

using NewDll = ::bh::d2::dll::Dll;

static Logger& GetLogger() {
	static Logger& logger = GetFileLogger(__FILEW__);
	return logger;
}

}  // namespace

std::vector<Patch*> Patch::Patches;

Patch::Patch(PatchType type, Dll dll, Offsets offsets, int function, int length)
: type(type), dll(dll), offsets(offsets), function(function), length(length) {
	oldCode = new BYTE[length];
	injected = false;
	Patches.push_back(this);
}

int Patch::GetDllOffset(Dll dll, int offset) {
	//Attempt to get the module of the given DLL
	NewDll newDll = ToNewDllEnum(dll);
	HMODULE hModule = GetHandle(newDll);

	//If the DLL isn't there, or failed to load, return.
	if (!hModule) {
		GetLogger().Fatal(
				__LINE__,
				"Unable to load DLL with value {}",
				static_cast<int>(newDll));
		return false;
	}

	//Check if it is an ordinal, if so, get the proper address.
	if (offset  < 0)
		return (DWORD)GetProcAddress(hModule, (LPCSTR)(-offset));

	//If just regular offset, add the two and be done!
	return ((DWORD)hModule) + offset;
}

bool Patch::WriteBytes(int address, int len, BYTE* bytes) {
	DWORD dwOld;

	if(!VirtualProtect((void*)address, len, PAGE_READWRITE, &dwOld))
		return FALSE;

	::memcpy((void*)address, bytes, len);
	return !!VirtualProtect((void*)address, len, dwOld, &dwOld);
}

bool Patch::Install() {

	//Check if we have already installed this patch.
	if (IsInstalled())
		return true;

	//Initalize variables for the exactly commands we are injecting.
	BYTE* code = new BYTE[length];
	DWORD protect;

	// Select an offset based on D2 version
	int offset = *(&offsets._113c + D2Version::GetGameVersionID());

	//Get the proper address that we are patching
	int address = GetDllOffset(dll, offset);

	//Read the old code to allow proper patch removal
	ReadProcessMemory(GetCurrentProcess(), (void*)address, oldCode, length, NULL);

	//Set the code with all NOPs by default
	if (type == 0x68) {
		memset(code, 0x00, length);
	}
	else {
		memset(code, 0x90, length);
	}

	if (type != NOP) {
		//Set the opcode
		code[0] = type;

		//Set the address to redirect to
		if (type == Call || type == Jump) {
			*(DWORD*)&code[1] = function - (address + 5);
		} else {
			code[1] = function;
		}
	}

	//Write the patch in
	VirtualProtect((VOID*)address, length, PAGE_EXECUTE_READWRITE, &protect);
	memcpy_s((VOID*)address, length, code, length);
	VirtualProtect((VOID*)address, length, protect, &protect);


	//Set that we successfully patched
	injected = true;

	return true;
}

bool Patch::Remove() {
	if (!IsInstalled())
		return true;

	// Select an offset based on D2 version
	int offset = *(&offsets._113c + D2Version::GetGameVersionID());

	//Get the proper address
	int address = GetDllOffset(dll, offset);
	DWORD protect;

	//Revert to the previous code
	VirtualProtect((VOID*)address, length, PAGE_EXECUTE_READWRITE, &protect);
	memcpy_s((VOID*)address, length, oldCode, length);
	VirtualProtect((VOID*)address, length, protect, &protect);


	injected = false;

	return true;
}

NewDll ToNewDllEnum(Dll oldDll) {
	switch (oldDll) {
		case Dll::BNCLIENT: {
			return NewDll::kBnClient;
		}

		case Dll::D2CLIENT: {
			return NewDll::kD2Client;
		}

		case Dll::D2CMP: {
			return NewDll::kD2Cmp;
		}

		case Dll::D2COMMON: {
			return NewDll::kD2Common;
		}

		case Dll::D2GAME: {
			return NewDll::kD2Game;
		}

		case Dll::D2GFX: {
			return NewDll::kD2Gfx;
		}

		case Dll::D2LANG: {
			return NewDll::kD2Lang;
		}

		case Dll::D2LAUNCH: {
			return NewDll::kD2Launch;
		}

		case Dll::D2MCPCLIENT: {
			return NewDll::kD2McpClient;
		}

		case Dll::D2MULTI: {
			return NewDll::kD2Multi;
		}

		case Dll::D2NET: {
			return NewDll::kD2Net;
		}

		case Dll::D2WIN: {
			return NewDll::kD2Win;
		}

		case Dll::FOG: {
			return NewDll::kFog;
		}

		case Dll::STORM: {
			return NewDll::kStorm;
		}
	}

	// This should never happen.
	GetLogger().Fatal(
			__LINE__,
			"Unhandled Dll with value {:d}.",
			static_cast<int>(oldDll));
	return static_cast<NewDll>(-1);
}

Dll ToOldDllEnum(NewDll newDll) {
	switch (newDll) {
		case NewDll::kBnClient: {
			return Dll::BNCLIENT;
		}

		case NewDll::kD2Client: {
			return Dll::D2CLIENT;
		}

		case NewDll::kD2Cmp: {
			return Dll::D2CMP;
		}

		case NewDll::kD2Common: {
			return Dll::D2COMMON;
		}

		case NewDll::kD2Game: {
			return Dll::D2GAME;
		}

		case NewDll::kD2Gfx: {
			return Dll::D2GFX;
		}

		case NewDll::kD2Lang: {
			return Dll::D2LANG;
		}

		case NewDll::kD2Launch: {
			return Dll::D2LAUNCH;
		}

		case NewDll::kD2McpClient: {
			return Dll::D2MCPCLIENT;
		}

		case NewDll::kD2Multi: {
			return Dll::D2MULTI;
		}

		case NewDll::kD2Net: {
			return Dll::D2NET;
		}

		case NewDll::kD2Win: {
			return Dll::D2WIN;
		}

		case NewDll::kFog: {
			return Dll::FOG;
		}

		case NewDll::kStorm: {
			return Dll::STORM;
		}
	}

	// This should never happen.
	GetLogger().Fatal(
			__LINE__,
			"Unhandled Dll with value {:d}.",
			static_cast<int>(newDll));
	return static_cast<Dll>(-1);
}
