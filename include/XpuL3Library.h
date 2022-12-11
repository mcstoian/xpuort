//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
/*
https://elfio.sourceforge.net/elfio.pdf
 */
//-------------------------------------------------------------------------------------
#pragma once
#include <map>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <XpuL4Driver.h>
#include <algorithm>
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct FunctionCode {
    uint32_t                byteCount;
    uint32_t                address;
    std::vector<uint8_t>    code;
};

struct FunctionInfo {
    std::vector<FunctionCode>    codes;
};


//-------------------------------------------------------------------------------------
class XpuL3Library {

public:
	XpuL3Library();

 	~XpuL3Library();

	void loadFunctions();
    void loadFunction(json::iterator _it);
    FunctionInfo* getFunction(std::string _name);

	void writeFunction(FunctionInfo* _functionInfo);
	void writeData(void* _address, uint32_t _length);

private:
    json libxpu;
	std::unordered_map<std::string, FunctionInfo> functionMap;
	XpuL4Driver* xpuL4Driver;

};
//-------------------------------------------------------------------------------------






