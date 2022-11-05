#pragma once

#include <windows.h>

#include <map>
#include <regex>
#include <string>

#include "../Module.h"

struct Control;

class Bnet : public Module {
	private:
		std::map<std::string, bool> bools;
		bool* showLastGame;
		bool* showLastPass;
		bool* nextInstead;
		bool* keepDesc;
		static unsigned int failToJoin;
		static std::wstring lastName;
		static std::wstring lastPass;
		static std::wstring lastDesc;
		static std::wregex reg;

	public:

		Bnet() : Module("Bnet") {};

		void OnLoad();
		void OnUnload();
		void LoadConfig();

		void OnGameJoin();
		void OnGameExit();

		void InstallPatches();
		void RemovePatches();

		std::map<std::string, bool>* GetBools() { return &bools; }

		static VOID __fastcall NextGamePatch(Control* box, BOOL (__stdcall *FunCallBack)(Control*, DWORD, DWORD));
		static VOID __fastcall NextPassPatch(Control* box, BOOL(__stdcall *FunCallBack)(Control*, DWORD, DWORD));
		static VOID __fastcall GameDescPatch(Control* box, BOOL(__stdcall *FunCallBack)(Control*, DWORD, DWORD));
		static void RemovePassPatch();
};

void FailToJoin_Interception();
void RemovePass_Interception();
