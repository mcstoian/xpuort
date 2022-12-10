//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <map>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <XpuL3Library.h>
#include <XpuL4Driver.h>
#include <algorithm>
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>

//-------------------------------------------------------------------------------------
XpuL2Resolver::XpuL2Resolver() {
    xpuL2Library = new XpuL3Library();
}

//-------------------------------------------------------------------------------------
void XpuL2Resolver::uploadFunction() {

}

//-------------------------------------------------------------------------------------
void XpuL2Resolver::uploadData(void* _address, uint32_t _length){

}

//-------------------------------------------------------------------------------------
