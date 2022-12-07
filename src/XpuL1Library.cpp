//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdint>
#include <XpuL1Library.h>

//-------------------------------------------------------------------------------------
XpuL1Library::XpuL1Library() {
    elfio reader;

    if(!reader.load("libxpu.so") ) {
        printf( "File [libxpu.so] is not found!\n");
        exit(1);
    }

    dump::header( std::cout, reader );
    dump::section_headers( std::cout, reader );
    dump::segment_headers( std::cout, reader );
    dump::symbol_tables( std::cout, reader );
    dump::notes( std::cout, reader );
    dump::modinfo( std::cout, reader );
    dump::dynamic_tags( std::cout, reader );
    dump::section_datas( std::cout, reader );
    dump::segment_datas( std::cout, reader );

}

//-------------------------------------------------------------------------------------
void XpuL1Library::writeFunction(std::string _name) {

}

//-------------------------------------------------------------------------------------
void XpuL1Library::writeData(void* _address, uint32_t _length){

}

//-------------------------------------------------------------------------------------
