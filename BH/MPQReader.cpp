#include "MPQReader.h"

#include <stddef.h>
#include <stdlib.h>
#include <windows.h>

#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "bh/d2/storm/function/file_close_archive.hpp"
#include "bh/d2/storm/function/file_close_file.hpp"
#include "bh/d2/storm/function/file_open_archive.hpp"
#include "bh/d2/storm/function/file_open_file_ex.hpp"
#include "Constants.h"
#include "D2Ptrs.h"

namespace {

namespace storm = ::bh::d2::storm;

}  // namespace

std::map<std::string, MPQData*> MpqDataMap;
std::string MpqVersion;

#define SFILE_INVALID_SIZE 0xFFFFFFFF
#define STREAM_FLAG_READ_ONLY 0x00000100  // Stream is read only

MPQArchive::MPQArchive(const char *filename) : name(filename), error(ERROR_SUCCESS) {
	if (!storm::SFileOpenArchive(filename, 0, STREAM_FLAG_READ_ONLY, &hMpq)) {
		error = GetLastError();
	}
}
MPQArchive::~MPQArchive() {
	if (hMpq != NULL) {
		storm::SFileCloseArchive(hMpq);
	}
}
HANDLE MPQArchive::GetHandle() {
	return hMpq;
}


MPQFile::MPQFile(MPQArchive *archive, const char *filename) : name(filename), error(ERROR_SUCCESS) {
	if (!storm::SFileOpenFileEx(archive->GetHandle(), filename, 0, &hMpqFile)) {
		error = GetLastError();
	}
}
MPQFile::~MPQFile() {
	if (hMpqFile != NULL) {
		storm::SFileCloseFile(hMpqFile);
	}
}
HANDLE MPQFile::GetHandle() {
	return hMpqFile;
}


MPQData::MPQData(MPQFile *file) : error(ERROR_SUCCESS) {
	DWORD dwBytes = 1;
	std::string buffer;
	char szBuffer[0x10000];
	while (dwBytes > 0) {
		STORM_SFileReadFile(file->GetHandle(), szBuffer, sizeof(szBuffer), &dwBytes, NULL);
		if (dwBytes > 0) {
			buffer.append(szBuffer, dwBytes);
		}
	}

	// TODO: need to remove \r, \n chars here
	if (error == ERROR_SUCCESS) {
		std::stringstream ss(buffer);
		std::string line;
		std::string field;
		if (std::getline(ss, line)) {  // read the header first
			std::stringstream hss(line);
			while (std::getline(hss, field, '\t')) {
				fields.push_back(field);
			}
			while (std::getline(ss, line)) {
				std::map<std::string, std::string> linedata;
				std::stringstream fss(line);
				for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end(); it++) {
					if (!std::getline(fss, field, '\t')) {
						field.clear();
					}
					linedata[(*it)] = field;
				}
				data.push_back(linedata);
			}
		}
	}
}
MPQData::~MPQData() {}

/*
 * To handle servers with customized mpq files, try to read
 * Patch_D2.mpq. If we can't load the dll or read the mpq, we will fall
 * back on a hard-coded list of the standard items.
 */
bool ReadMPQFiles(std::string fileName) {
	int successfulFileCount = 0, desiredFileCount = 0;
	HANDLE pMutex = CreateMutex(NULL, true, "Global\\BH_PATCH_D2_MPQ_MUTEX");
	WaitForSingleObject(
			pMutex,    // handle to mutex
			INFINITE);  // no time-out interval

	MPQArchive archive(fileName.c_str());

	const int NUM_MPQS = 15;
	std::string mpqFiles[NUM_MPQS] = {
		"UniqueItems",
		"Armor",
		"Weapons",
		"Misc",
		"ItemTypes",
		"ItemStatCost",
		"Properties",
		"Runes",
		"SetItems",
		"skills",
		"MagicPrefix",
		"MagicSuffix",
		"RarePrefix",
		"RareSuffix",
		"CharStats"
	};
	if (archive.error == ERROR_SUCCESS) {
		for (int i = 0; i < NUM_MPQS; i++){
			std::string path = "data\\global\\excel\\" + mpqFiles[i] + ".txt";
			MPQFile mpqFile(&archive, path.c_str()); desiredFileCount++;
			if (mpqFile.error == ERROR_SUCCESS) {
				successfulFileCount++;
				std::string key = mpqFiles[i];
				std::transform(key.begin(), key.end(), key.begin(), ::tolower);
				MpqDataMap[key] = new MPQData(&mpqFile);
			}
		}
	}
	// read mpq version
	std::string path = "data\\version.txt";
	MPQFile mpqFile(&archive, path.c_str());
	if (mpqFile.error == ERROR_SUCCESS) {
		MPQData mpqversion(&mpqFile);
		MpqVersion = mpqversion.fields[0];
	}

	ReleaseMutex(pMutex);
	CloseHandle(pMutex);
	return true;
}

void FindAncestorTypes(std::string type, std::set<std::string>& ancestors, std::map<std::string, std::string>& map1, std::map<std::string, std::string>& map2) {
	ancestors.insert(type);
	std::map<std::string, std::string>::iterator it1 = map1.find(type);
	if (it1 != map1.end()) {
		FindAncestorTypes(it1->second, ancestors, map1, map2);
	}
	std::map<std::string, std::string>::iterator it2 = map2.find(type);
	if (it2 != map2.end()) {
		FindAncestorTypes(it2->second, ancestors, map1, map2);
	}
}

unsigned int AssignClassFlags(std::string type, std::set<std::string>& ancestors, unsigned int flags) {
	if (ancestors.find("amaz") != ancestors.end()) {
		flags |= ITEM_GROUP_AMAZON_WEAPON;
	} else if (ancestors.find("barb") != ancestors.end()) {
		flags |= ITEM_GROUP_BARBARIAN_HELM;
	} else if (ancestors.find("necr") != ancestors.end()) {
		flags |= ITEM_GROUP_NECROMANCER_SHIELD;
	} else if (ancestors.find("pala") != ancestors.end()) {
		flags |= ITEM_GROUP_PALADIN_SHIELD;
	} else if (ancestors.find("sorc") != ancestors.end()) {
		flags |= ITEM_GROUP_SORCERESS_ORB;
	} else if (ancestors.find("assn") != ancestors.end()) {
		flags |= ITEM_GROUP_ASSASSIN_KATAR;
	} else if (ancestors.find("drui") != ancestors.end()) {
		flags |= ITEM_GROUP_DRUID_PELT;
	}
	return flags;
}
