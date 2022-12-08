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
#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#define ELFIO_NO_INTTYPES
#endif

#include <iostream>
#include <elfio/elfio_dump.hpp>

using namespace ELFIO;

//-------------------------------------------------------------------------------------
class XpuL1Library {

public:
	XpuL1Library();

 	~XpuL1Library();

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
};
//-------------------------------------------------------------------------------------






