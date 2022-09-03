#include "ModuleManager.h"

#include <wchar.h>
#include <windows.h>

#include <algorithm>
#include <map>
#include <string>

#include "../BH.h"
#include "../D2Helpers.h"
#include "Module.h"

ModuleManager::ModuleManager() {

}

ModuleManager::~ModuleManager() {
	for (auto it = moduleList.begin(); it != moduleList.end(); ++it) {
		Module* module = (*it).second;
		delete module;
	}
	moduleList.clear();
}

void ModuleManager::FixName(std::string& name)
{
	std::transform(name.begin(), name.end(), name.begin(), tolower);
	std::replace(name.begin(), name.end(), ' ', '-');
}

void ModuleManager::Add(Module* module) {
	// Add to list of modules
	std::string name = module->GetName();
	FixName(name);
	moduleList[name] = module;
}

Module* ModuleManager::Get(std::string name) {
	// Get a pointer to a module
	if (moduleList.count(name) > 0) {
		return moduleList[name];
	}
	return NULL;
}

void ModuleManager::Remove(Module* module) {
	// Remove module from list
	std::string name = module->GetName();
	FixName(name);
	moduleList.erase(name);

	delete module;
}

void ModuleManager::LoadModules() {
	for (std::map<std::string, Module*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		(*it).second->Load();
	}
}

void ModuleManager::UnloadModules() {
	for (std::map<std::string, Module*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		(*it).second->Unload();
	}
}

void ModuleManager::ReloadConfig() {
	for (std::map<std::string, Module*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		(*it).second->LoadConfig();
	}
}

void ModuleManager::MpqLoaded() {
	for (std::map<std::string, Module*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		(*it).second->MpqLoaded();
	}
}

bool ModuleManager::UserInput(wchar_t* module, wchar_t* msg, bool fromGame) {
	bool block = false;
	std::string name;
	std::wstring modname(module);
	name.assign(modname.begin(), modname.end());
	transform(name.begin(), name.end(), name.begin(), ::tolower);

	if (name.compare("reload") == 0)
	{
		ReloadConfig();
		Print("\xFF" "c4BH:" "\xFF" "c0 Successfully reloaded configuration.");
		return true;
	}

	if (name.compare("save") == 0) {
		BH::config->Write();
		Print("\xFF" "c4BH:" "\xFF" "c0 Successfully saved configuration.");
	}

	for (std::map<std::string, Module*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		if (name.compare((*it).first) == 0) {
			__raise it->second->UserInput(msg, fromGame, &block);
		}
	}
	return block;
}
