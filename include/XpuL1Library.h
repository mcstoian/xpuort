//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
/*
	example of basic program for the xpu system.
		write program file into xpu
		get data in into the xpu system using dma mechanism
		do computations (done automatically)
		get data out from the xpu system using dma mechanism
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

	void writeFunction(std::string _name);
	void writeData(void* _address, uint32_t _length);
};
//-------------------------------------------------------------------------------------






