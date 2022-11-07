#include "Config.h"

#include <stddef.h>

#include <algorithm>
#include <fstream>
#include <ios>
#include <list>
#include <map>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "bh/common/input.hpp"
#include "bh/common/string_util.hpp"
#include "BH.h"
#include "Common.h"

namespace {

using ::common::input::VirtualKey;
using ::common::str_util::IsHex;
using ::common::str_util::ToBool;
using ::common::str_util::ToInteger;
using ::common::str_util::Trim;

}  // namespace

/* Parse()
Parse the configuration file and stores the results in a key->value pair.
Can be called multiple times so you can reload the configuration.
*/
bool Config::Parse() {

	//If this is a config with a pre-set of values, can't parse!
	if (configName.length() == 0)
		return false;

	//Open the configuration file
	std::fstream file(BH::path + configName);
	if (!file.is_open())
		return false;

	//Lock Critical Section then wipe contents incase we are reloading.
	contents.erase(contents.begin(), contents.end());
	orderedKeyVals.clear();

	//Begin to loop the configuration file one line at a time.
	std::string line;
	int lineNo = 0;
	while (std::getline(file, line)) {
		lineNo++;
		std::string comment;
		//Remove any comments from the config
		if (line.find("//") != std::string::npos) {
			comment = line.substr(line.find("//"));
			line = line.erase(line.find("//"));
		}

		//Insure we have something in the line now.
		if (line.length() == 0)
			continue;

		//Grab the Key and Value

		ConfigEntry entry;
		entry.line = lineNo;

		// Parse the key and value.
		size_t keyValueDelimiterIndex = line.find_first_of(":");

		std::string_view rawKeyStr(line.c_str(), keyValueDelimiterIndex);
		entry.key = Trim(rawKeyStr);

		std::string_view rawValueStr(line);
		rawValueStr.remove_prefix(keyValueDelimiterIndex + 1);
		entry.value = Trim(rawValueStr);

		entry.comment = line.substr(keyValueDelimiterIndex + 1, line.find(entry.value) - keyValueDelimiterIndex - 1);
		entry.pointer = NULL;

		//Store them!
		contents.insert(std::pair<std::string, ConfigEntry>(entry.key, entry));
		orderedKeyVals.push_back(std::pair<std::string, std::string>(entry.key, entry.value));
	}
	file.close();
	return true;
}

bool Config::Write() {
	//If this is a config with a pre-set of values, can't parse!
	if (configName.length() == 0)
		return false;

	//Open the configuration file
	std::fstream file(BH::path + configName);
	if (!file.is_open())
		return false;

	//Read in the configuration value
	std::vector<std::string> configLines;
	char line[2048];
	while (!file.eof()) {
		file.getline(line, 2048);
		configLines.push_back(line);
	}
	file.close();

	std::map<ConfigEntry, std::string> changed;
	for (std::map<std::string, ConfigEntry>::iterator it = contents.begin(); it != contents.end(); ++it) {
		std::string newValue;
		if (!HasChanged((*it).second, newValue))
			continue;
		std::pair<ConfigEntry, std::string> change;
		change.first = (*it).second;
		change.second = newValue.c_str();
		changed.insert(change);
	}

	if (changed.size() == 0)
		return true;

	for (std::vector<std::string>::iterator it = configLines.begin(); it < configLines.end(); it++) {
		//Remove any comments from the config
		std::string comment;
		if ((*it).find_first_of("//") != std::string::npos) {
			comment = (*it).substr((*it).find_first_of("//"));
			(*it) = (*it).erase((*it).find_first_of("//"));
		}

		//Insure we have something in the line now.
		if ((*it).length() == 0) {
			*it = comment;
			continue;
		}

		size_t keyValueDelimiterIndex = it->find_first_of(":");
		std::string_view rawKeyStr(it->c_str(), keyValueDelimiterIndex);
		std::string keyStr = Trim(rawKeyStr);

		*it = *it + comment;

		for (std::map<ConfigEntry, std::string>::iterator cit = changed.begin(); cit != changed.end(); ++cit)
		{
			if ((*cit).first.key.compare(keyStr) != 0)
				continue;

			if ((*cit).second.size() == 0)
			{
				*it = "//Purge";
				contents[keyStr].value = "";
				changed.erase((*cit).first);
				break;
			}

			std::stringstream newLine;
			newLine << keyStr << ":" << (*cit).first.comment << (*cit).second << comment;
			*it = newLine.str();
			contents[keyStr].value = (*cit).second;

			changed.erase((*cit).first);
			break;
		}
	}

	for (std::map<ConfigEntry, std::string>::iterator cit = changed.begin(); cit != changed.end(); ++cit)
	{
		std::stringstream newConfig;

		newConfig << (*cit).first.key << ": " << (*cit).second;

		configLines.push_back(newConfig.str());
	}

	std::ofstream outFile(BH::path + configName);
	for (std::vector<std::string>::iterator it = configLines.begin(); it < configLines.end(); it++) {
		if ((*it).compare("//Purge") == 0)
			continue;

		if (std::next(it) == configLines.end())
			outFile << (*it);
		else
			outFile << (*it) << std::endl;
	}
	outFile.close();

	return true;
}

std::string Config::GetConfigName() {
	return BH::path + configName;
}

void Config::SetConfigName(std::string name) {
	configName = name;
}

/* ReadBoolean(std::string key, bool value)
*	Reads in a boolean from the key-pair.
*/
bool Config::ReadBoolean(std::string key, bool& value) {
	//Check if configuration value exists
	if (contents.find(key) == contents.end()) {
		contents[key].key = key;
		contents[key].value = value;
	}

	contents[key].type = CTBoolean;
	contents[key].pointer = (void*)&value;

	//Convert string to boolean
	if (ToBool(contents[key].value).value_or(false))
		value = true;
	else
		value = false;
	return value;
}

/* ReadInt(std::string key, int value)
*	Reads in a decimal or hex(which is converted to decimal) from the key-pair.
*/
int Config::ReadInt(std::string key, int& value) {
	//Check if configuration value exists
	if (contents.find(key) == contents.end()) {
		contents[key].key = key;
		contents[key].value = value;
	}

	contents[key].type = CTInt;
	contents[key].pointer = (void*)&value;

	std::optional valueOptional = ToInteger<int>(contents[key].value);
	value = valueOptional.value_or(0);
	return value;
}

/* ReadInt(std::string key, int value)
*	Reads in a decimal or hex(which is converted to decimal) from the key-pair.
*/
unsigned int Config::ReadInt(std::string key, unsigned int& value) {
	//Check if configuration value exists
	if (contents.find(key) == contents.end()) {
		contents[key].key = key;
		contents[key].value = value;
	}

	contents[key].type = CTInt;
	contents[key].pointer = &value;

	std::optional valueOptional =
			ToInteger<unsigned int>(contents[key].value);
	value = valueOptional.value_or(0);
	return value;
}

std::string Config::ReadString(std::string key, std::string &value) {
	//Check if configuration value exists
	if (contents.find(key) == contents.end())
	{
		contents[key].key = key;
		contents[key].value = value;
	}

	contents[key].pointer = &value;
	contents[key].type = CTString;

	value = contents[key].value;
	return value;
}

/* ReadToggle(std::string key, std::string toggle, bool state)
*	Reads in a toggle from the key->pair
*	Config Example:
*		Key: True, VK_A
*/
Toggle Config::ReadToggle(std::string key, std::string toggle, bool state, Toggle& value) {
	//Check if configuration value exists.
	Toggle ret;
	if (contents.find(key) == contents.end()) {
		contents[key].key = key;
		contents[key].value = ((state) ? "True, " : "False, ") + toggle;
	}

	contents[key].toggle = &value;
	contents[key].type = CTToggle;

	size_t stateVkeydelimiterIndex = contents[key].value.find_first_of(",");

	// Read state string.
	std::string_view rawStateStr(
			contents[key].value.c_str(), stateVkeydelimiterIndex);
	std::string stateStr = Trim(rawStateStr);

	// Read virtual-key string and get mapped code.
	std::string_view rawVirtualKeyStr(contents[key].value);
	rawVirtualKeyStr.remove_prefix(stateVkeydelimiterIndex + 1);
	std::string virtualKeyStr = Trim(rawVirtualKeyStr);
	std::optional<VirtualKey> virtualKeyOptional =
			VirtualKey::GetFromSymbolName(virtualKeyStr);
	VirtualKey virtualKey =
			virtualKeyOptional.value_or(VirtualKey::GetUnset());

	ret.toggle = virtualKey.code;
	ret.state = ToBool(stateStr).value_or(false);

	value = ret;
	return ret;
}

/* ReadKey(std::string key, std::string toggle)
*	Reads in a key from the key->pair.
*/
unsigned int Config::ReadKey(std::string key, std::string toggle, unsigned int &value) {
	//Check if configuration value exists.
	if (contents.find(key) == contents.end()) {
		contents[key].key = key;
		contents[key].value = toggle;
	}

	contents[key].pointer = &value;
	contents[key].type = CTKey;

	// Grab the proper key code and make sure it's valid
	std::optional<VirtualKey> virtualKeyOptional =
			VirtualKey::GetFromSymbolName(contents[key].value);
	if (!virtualKeyOptional.has_value()) {
		virtualKeyOptional = VirtualKey::GetFromSymbolName(toggle);
	}
	VirtualKey virtualKey = virtualKeyOptional.value_or(VirtualKey::GetUnset());

	value = virtualKey.code;

	return virtualKey.code;
}

/* ReadArray(std::string key)
*	Reads in a index-based array from the array
*/
std::vector<std::string> Config::ReadArray(std::string key, std::vector<std::string> &value) {
	int currentIndex = 0;
	value.clear();
	while (true) {
		std::stringstream index;
		index << currentIndex;
		std::string currentKey = key + "[" + index.str() + "]";
		if (contents.find(currentKey) == contents.end())
			break;
		value.push_back(contents[currentKey].value);
		contents[currentKey].pointer = &value;
		contents[currentKey].type = CTArray;
		currentIndex++;
	}
	return value;
}

/* ReadGroup(std::string key)
*	Reads in a map from the key->pair
*	Config Example:
*		Value[Test]: 0
*		Value[Pickles]: 1
*/
std::map<std::string, std::string> Config::ReadAssoc(std::string key, std::map<std::string, std::string> &value) {

	for (std::map<std::string, ConfigEntry>::iterator it = contents.begin(); it != contents.end(); it++) {
		if (!(*it).first.find(key + "[")) {
			std::pair<std::string, std::string> assoc;
			//Pull the value from between the []'s
			assoc.first = (*it).first.substr((*it).first.find("[") + 1, (*it).first.length() - (*it).first.find("[") - 2);
			// Check if key is already defined in map
			if (value.find(assoc.first) == value.end()) {
				assoc.second = (*it).second.value;
				value.insert(assoc);
			}
			else {
				value[key] = (*it).second.value;
			}

			(*it).second.pointer = &value;
			(*it).second.type = CTAssoc;
		}
	}

	return value;
}

std::map<std::string, bool> Config::ReadAssoc(std::string key, std::map<std::string, bool> &value) {

	for (std::map<std::string, ConfigEntry>::iterator it = contents.begin(); it != contents.end(); it++) {
		if (!(*it).first.find(key + "[")) {
			std::pair<std::string, bool> assoc;
			assoc.first = (*it).first.substr((*it).first.find("[") + 1, (*it).first.length() - (*it).first.find("[") - 2);
			transform(assoc.first.begin(), assoc.first.end(), assoc.first.begin(), ::tolower);

			if (value.find(assoc.first) == value.end()) {
				assoc.second = ToBool((*it).second.value).value_or(false);
				value.insert(assoc);
			}
			else {
				value[assoc.first] = ToBool((*it).second.value).value_or(false);
			}

			(*it).second.pointer = &value;
			(*it).second.type = CTAssocBool;
		}
	}

	return value;
}

std::map<std::string, unsigned int> Config::ReadAssoc(std::string key, std::map<std::string, unsigned int> &value) {

	for (std::map<std::string, ConfigEntry>::iterator it = contents.begin(); it != contents.end(); it++) {
		if ((*it).first.find(key + "[") != std::string::npos) {
			std::pair<std::string, unsigned int> assoc;
			//Pull the value from between the []'s
			assoc.first = (*it).first.substr((*it).first.find("[") + 1, (*it).first.length() - (*it).first.find("[") - 2);
			//Simply store the value that was after the :
			std::optional mappingValueOptional =
					ToInteger<unsigned int>(it->second.value);
			if (!mappingValueOptional.has_value()) {
				continue;
			}
			assoc.second = *mappingValueOptional;

			if (value.find(assoc.first) == value.end()) {
				value.insert(assoc);
			}
			else {
				value[assoc.first] = assoc.second;
			}

			(*it).second.pointer = &value;
			(*it).second.type = CTAssocInt;
		}
	}

	return value;
}

std::vector<std::pair<std::string, std::string>> Config::ReadMapList(std::string key, std::vector<std::pair<std::string, std::string>>& values) {

	for (std::vector<std::pair<std::string, std::string>>::iterator it = orderedKeyVals.begin(); it != orderedKeyVals.end(); it++) {
		if (!(*it).first.find(key + "[")) {
			std::pair<std::string, std::string> assoc;
			//Pull the value from between the []'s
			assoc.first = (*it).first.substr((*it).first.find("[") + 1, (*it).first.length() - (*it).first.find("[") - 2);
			//Also store the value
			assoc.second = (*it).second;
			values.push_back(assoc);
		}
	}

	return values;
}

std::list<std::string> Config::GetDefinedKeys() {
	std::list<std::string> ret;

	for (std::map<std::string, ConfigEntry>::iterator it = contents.begin(); it != contents.end(); it++) {
		std::string key = (*it).first;

		if (key.find("[") != std::string::npos)
			key = key.substr(0, key.find("["));

		ret.push_back(key);
	}
	ret.unique();
	return ret;
}

bool Config::HasChanged(ConfigEntry entry, std::string& value) {
	if (entry.type != CTToggle && entry.pointer == NULL)
		return false;

	switch (entry.type) {
	case CTBoolean: {
		bool currentBool = *((bool*)entry.pointer);
		bool storedBool = ToBool(entry.value).value_or(false);

		if (storedBool == currentBool)
			return false;

		value = currentBool ? "True" : "False";
		return true;
	}
	case CTInt: {
		int currentInt = *((int*)entry.pointer);

		int storedInt = 0;
		std::stringstream stream;
		storedInt = ToInteger<int>(entry.value).value_or(0);

		if (currentInt == storedInt)
			return false;

		value.clear();
		if (IsHex<int>(entry.value)) {
			stream << std::hex;
			value = "0x";
		}
		stream << currentInt;
		value += stream.str();
		return true;
	}
	case CTString: {
		std::string currentString = *((std::string*)entry.pointer);

		if (currentString.compare(entry.value) == 0)
			return false;

		value = currentString;
		return true;
	}
	case CTArray: {
		using size_type = std::vector<std::string>::size_type;

		std::vector<std::string> valTest = *((std::vector<std::string>*)entry.pointer);
		std::string ind = entry.key.substr(entry.key.find("[") + 1, entry.key.length() - entry.key.find("[") - 2);
		size_type index = ToInteger<size_type>(ind).value_or(0);

		if (index >= valTest.size()) {
			value = "";
			return true;
		}

		std::string currentString = valTest.at(index);

		if (currentString.compare(entry.value) == 0)
			return false;

		value = currentString;
		return true;
	}

	case CTAssoc: {
		std::string assocKey = entry.key.substr(entry.key.find("[") + 1, entry.key.length() - entry.key.find("[") - 2);
		std::map<std::string, std::string> valTest = *((std::map<std::string, std::string>*)entry.pointer);

		std::string currentString = valTest[assocKey];

		if (currentString.compare(entry.value) == 0)
			return false;

		value = currentString;
		return true;
	}
	case CTAssocBool: {
		std::string assocKey = entry.key.substr(entry.key.find("[") + 1, entry.key.length() - entry.key.find("[") - 2);
		std::transform(assocKey.begin(), assocKey.end(), assocKey.begin(), ::tolower);
		std::map<std::string, bool> valTest = *((std::map<std::string, bool>*)entry.pointer);

		bool currentBool = valTest[assocKey];

		if (currentBool == ToBool(entry.value).value_or(false))
			return false;

		value = currentBool ? "True" : "False";
		return true;
	}
	case CTAssocInt: {
		std::string assocKey = entry.key.substr(entry.key.find("[") + 1, entry.key.length() - entry.key.find("[") - 2);
		std::map<std::string, unsigned int> valTest = *((std::map<std::string, unsigned int>*)entry.pointer);
		int currentInt = valTest[assocKey];

		int storedInt = 0;
		std::stringstream stream;
		storedInt = ToInteger<int>(entry.value).value_or(0);

		if (currentInt == storedInt)
			return false;

		value.clear();
		if (IsHex<int>(entry.value)) {
			stream << std::hex;
			value = "0x";
		}
		stream << currentInt;
		value += stream.str();
		return true;
	}
	case CTToggle: {
		size_t stateVkeydelimiterIndex = entry.value.find_first_of(",");

		// Read state string for the old Toggle.
		std::string_view rawOldStateStr(
				entry.value.c_str(), stateVkeydelimiterIndex);
		std::string oldStateStr = Trim(rawOldStateStr);
		bool oldState = ToBool(oldStateStr).value_or(false);

		// Get the virtual-key for the old Toggle.
		std::string_view rawOldVirtualKeyStr(
				entry.value.c_str(), stateVkeydelimiterIndex + 1);
		std::string oldVirtualKeyStr = Trim(rawOldVirtualKeyStr);
		std::optional<VirtualKey> oldVirtualKeyOptional =
				VirtualKey::GetFromSymbolName(oldVirtualKeyStr);
		VirtualKey oldVirtualKey =
				oldVirtualKeyOptional.value_or(VirtualKey::GetUnset());

		if (entry.toggle->toggle == oldVirtualKey.code && entry.toggle->state == oldState)
			return false;

		std::stringstream stream;
		const VirtualKey& newKey = VirtualKey::GetFromCode(entry.toggle->toggle);

		stream << ((entry.toggle->state) ? "True" : "False") << ", " << newKey.symbol_name;

		value = stream.str();
		return true;
	}
	case CTKey: {
		unsigned int currentKeyCode = *((unsigned int*)entry.pointer);

		// Get the virtual-key for the old Key.
		std::optional<VirtualKey> oldVirtualKeyOptional =
				VirtualKey::GetFromSymbolName(entry.value);
		VirtualKey oldVirtualKey =
				oldVirtualKeyOptional.value_or(VirtualKey::GetUnset());

		if (oldVirtualKey.code == currentKeyCode) {
			return false;
		}

		const VirtualKey& newVirtualKey = VirtualKey::GetFromCode(currentKeyCode);
		value = newVirtualKey.symbol_name;
		return true;
	}
	}
	return false;
}
