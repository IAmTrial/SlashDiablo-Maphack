#include "Bnet.h"

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <windows.h>

#include <map>
#include <regex>
#include <string>

#include "../../BH.h"
#include "../../Common.h"
#include "../../Common/StringUtil.h"
#include "../../D2Ptrs.h"
#include "../../D2Structs.h"
#include "../../Patch.h"
#include "../Module.h"

namespace {

using ::common::str_util::ToInteger;
using ::common::str_util::wide::FromUtf8;

}  // namespace

unsigned int Bnet::failToJoin;
std::wstring Bnet::lastName;
std::wstring Bnet::lastPass;
std::wstring Bnet::lastDesc;
std::wregex Bnet::reg(L"^(.*?)(\\d+)$");

Patch* nextGame1 = new Patch(Call, D2MULTI, { 0x14D29, 0xADAB }, (int)Bnet::NextGamePatch, 5);
Patch* nextGame2 = new Patch(Call, D2MULTI, { 0x14A0B, 0xB5E9 }, (int)Bnet::NextGamePatch, 5);
Patch* nextPass1 = new Patch(Call, D2MULTI, { 0x14D64, 0xADE6 }, (int)Bnet::NextPassPatch, 5);
Patch* nextPass2 = new Patch(Call, D2MULTI, { 0x14A46, 0xB624 }, (int)Bnet::NextPassPatch, 5);

Patch* gameDesc = new Patch(Call, D2MULTI, { 0x14D8F, 0xB64F }, (int)Bnet::GameDescPatch, 5);

Patch* ftjPatch = new Patch(Call, D2CLIENT, { 0x4363E, 0x443FE }, (int)FailToJoin_Interception, 6);
Patch* removePass = new Patch(Call, D2MULTI, { 0x1250, 0x1AD0 }, (int)RemovePass_Interception, 5);

void Bnet::OnLoad() {
	showLastGame = &bools["Autofill Last Game"];
	*showLastGame = true;
	
	showLastPass = &bools["Autofill Last Password"];
	*showLastPass = true;

	nextInstead = &bools["Autofill Next Game"];
	*nextInstead = true;

	keepDesc = &bools["Autofill Description"];
	*keepDesc = true;

	failToJoin = 4000;
	LoadConfig();
}

void Bnet::LoadConfig() {
	BH::config->ReadBoolean("Autofill Last Game", *showLastGame);
	BH::config->ReadBoolean("Autofill Last Password", *showLastPass);
	BH::config->ReadBoolean("Autofill Next Game", *nextInstead);
	BH::config->ReadBoolean("Autofill Description", *keepDesc);
	BH::config->ReadInt("Fail To Join", failToJoin);

	InstallPatches();
}

void Bnet::InstallPatches() {
	if (*showLastGame || *nextInstead) {
		nextGame1->Install();
		nextGame2->Install();
	}

	if (*showLastPass) {
		nextPass1->Install();
		nextPass2->Install();
		removePass->Install();
	}

	if (*keepDesc) {
		gameDesc->Install();
	}

	if (failToJoin > 0 && !D2CLIENT_GetPlayerUnit())
		ftjPatch->Install();
}

void Bnet::RemovePatches() {
	nextGame1->Remove();
	nextGame2->Remove();

	nextPass1->Remove();
	nextPass2->Remove();

	gameDesc->Remove();

	ftjPatch->Remove();
	removePass->Remove();
}

void Bnet::OnUnload() {
	RemovePatches();
}

void Bnet::OnGameJoin() {
	if ( strlen((*p_D2LAUNCH_BnData)->szGameName) > 0)
		lastName = FromUtf8((*p_D2LAUNCH_BnData)->szGameName);

	if ( strlen((*p_D2LAUNCH_BnData)->szGamePass) > 0)
		lastPass = FromUtf8((*p_D2LAUNCH_BnData)->szGamePass);
	else
		lastPass.clear();
	
	if ( strlen((*p_D2LAUNCH_BnData)->szGameDesc) > 0)
		lastDesc = FromUtf8((*p_D2LAUNCH_BnData)->szGameDesc);
	else
		lastDesc.clear();

	RemovePatches();
}

void Bnet::OnGameExit() {
	if (*nextInstead) {
		std::wsmatch match;
		if (std::regex_search(Bnet::lastName, match, Bnet::reg) && match.size() == 3) {
			std::wstring name = match.format(L"$1");
			if (name.length() != 0) {
				int count = ToInteger<int>(match.format(L"$2").c_str()).value_or(0);

				//Restart at 1 if the next number would exceed the max game name length of 15
				if (lastName.length() == 15) {
					int maxCountLength = 15 - name.length();
					int countLength = 1;
					int tempCount = count + 1;
					while (tempCount > 9) {
						countLength++;
						tempCount /= 10;
					}
					if (countLength > maxCountLength) {
						count = 1;
					} else {
						count++;
					}
				} else {
					count++;
				}
				lastName = std::format(L"{}{}", name, count);
				lastName.resize(15);
			}
		}
	}

	InstallPatches();
}

VOID __fastcall Bnet::NextGamePatch(Control* box, BOOL (__stdcall *FunCallBack)(Control*, DWORD, DWORD)) {
	if (Bnet::lastName.empty())
		return;

	D2WIN_SetControlText(box, Bnet::lastName.c_str());
	D2WIN_SelectEditBoxText(box);

	// original code
	D2WIN_SetEditBoxProc(box, FunCallBack);
}

VOID __fastcall Bnet::NextPassPatch(Control* box, BOOL(__stdcall *FunCallBack)(Control*, DWORD, DWORD)) {
	if (Bnet::lastPass.empty())
		return;
	
	D2WIN_SetControlText(box, Bnet::lastPass.c_str());
	
	// original code
	D2WIN_SetEditBoxProc(box, FunCallBack);
}

VOID __fastcall Bnet::GameDescPatch(Control* box, BOOL(__stdcall *FunCallBack)(Control*, DWORD, DWORD)) {
	if (Bnet::lastDesc.empty())
		return;
	
	D2WIN_SetControlText(box, Bnet::lastDesc.c_str());
	
	// original code
	D2WIN_SetEditBoxProc(box, FunCallBack);
}

void __declspec(naked) RemovePass_Interception() {
	__asm {
		PUSHAD
		CALL [Bnet::RemovePassPatch]
		POPAD

		; Original code
		XOR EAX, EAX
		SUB ECX, 01
		RET
	}
}

void Bnet::RemovePassPatch() {
	Control* box = *p_D2MULTI_PassBox;
	if (Bnet::lastPass.empty() || box == nullptr) {
		return;
	}

	D2WIN_SetControlText(box, L"");
}

void __declspec(naked) FailToJoin_Interception()
{
	/*
	Changes the amount of time, in milliseconds, that we wait for the loading
	door to open before the client confirms that it failed to join the game.
	*/
	__asm
	{
		cmp esi, Bnet::failToJoin;
		ret;
	}
}
