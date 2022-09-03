#pragma once

#include <windows.h>

#include <map>
#include <set>
#include <string>
#include <vector>

class MPQArchive {
public:
	std::string name;
	int error;
	MPQArchive(const char *filename);
	~MPQArchive();
	HANDLE GetHandle();
private:
	HANDLE hMpq;
};

class MPQFile {
public:
	std::string name;
	int error;
	MPQFile(MPQArchive *archive, const char *filename);
	~MPQFile();
	HANDLE GetHandle();
private:
	HANDLE hMpqFile;
};

class MPQData {
public:
	int error;
	MPQData(MPQFile *file);
	~MPQData();
	std::vector<std::string> fields;
	std::vector<std::map<std::string, std::string>> data;
private:
};

extern std::map<std::string, MPQData*> MpqDataMap;
extern std::string MpqVersion;

extern "C" __declspec(dllexport) bool ReadMPQFiles(std::string fileName);
void FindAncestorTypes(std::string type, std::set<std::string>& ancestors, std::map<std::string, std::string>& map1, std::map<std::string, std::string>& map2);
unsigned int AssignClassFlags(std::string type, std::set<std::string>& ancestors, unsigned int flags);
