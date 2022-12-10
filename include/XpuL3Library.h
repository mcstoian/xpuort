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
#include <elfio/elfio.hpp>
#include <elfio/elfio_dump.hpp>

#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#define ELFIO_NO_INTTYPES
#endif

#include <elfio/elfio_dump.hpp>
#include <XpuL4Driver.h>

using namespace ELFIO;

struct FunctionInfo {
      Elf64_Addr 			address;
      Elf_Xword 			length;
};

//-------------------------------------------------------------------------------------
class XpuL3Library {

public:
	XpuL3Library();

 	~XpuL3Library();

	void loadFunctions();
	void loadFunction(Elf_Xword no,
                              const std::string& name,
                              Elf64_Addr         value,
                              Elf_Xword          size,
                              unsigned char      bind,
                              unsigned char      type,
                              Elf_Half           section,
                              unsigned int       elf_class );
	void writeFunction(std::string _name);
	void writeData(void* _address, uint32_t _length);

private:
    elfio reader;
	std::unordered_map<std::string, FunctionInfo> functionMap;
	XpuL4Driver* xpuL4Driver;

};
//-------------------------------------------------------------------------------------






