//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <any>
#include <map>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <XpuL3Library.h>
#include <XpuL2Resolver.h>
#include <algorithm>
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>

//-------------------------------------------------------------------------------------
void do_adjusted_input() {
    std::cout << "[do_adjusted_input]: " << std::endl;
}

//-------------------------------------------------------------------------------------
void convolution_output() {
    std::cout << "[convolution_output]: " << std::endl;
}


//-------------------------------------------------------------------------------------
XpuL2Resolver::XpuL2Resolver() {
    xpuL3Library = new XpuL3Library();
    internallyResolvedFunctionMap["adjusted_input"] = do_adjusted_input;
    internallyResolvedFunctionMap["convolution_output"] = convolution_output;
}

//-------------------------------------------------------------------------------------
void XpuL2Resolver::resolve(std::string _name) {
    std::unordered_map<std::string, std::any>::const_iterator _iterator = internallyResolvedFunctionMap.find(_name);
    if(_iterator == internallyResolvedFunctionMap.end()){
        FunctionInfo* _functionInfo = xpuL3Library -> getFunction(_name);
        if(_functionInfo == NULL){
            std::cout << "Cannot resolve function: " << _name << std::endl;
            exit(1);
        } else {
            xpuL3Library -> writeFunction(_functionInfo);
        }
    } else {
        std::any_cast <void (*) ()> (_iterator->second) ();
    }
}

//-------------------------------------------------------------------------------------
void XpuL2Resolver::uploadFunction(std::string _name) {

}

//-------------------------------------------------------------------------------------
void XpuL2Resolver::uploadData(void* _address, uint32_t _length){

}

//-------------------------------------------------------------------------------------
