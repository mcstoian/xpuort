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
#include <algorithm>
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <elfio/elfio.hpp>
#include <elfio/elfio_dump.hpp>

#define DUMP_DEC_FORMAT( width ) \
    std::setw( width ) << std::setfill( ' ' ) << std::dec << std::right
#define DUMP_HEX0x_FORMAT( width ) \
    "0x" << std::setw( width ) << std::setfill( '0' ) << std::hex << std::right
#define DUMP_HEX_FORMAT( width ) \
    std::setw( width ) << std::setfill( '0' ) << std::hex << std::right
#define DUMP_STR_FORMAT( width ) \
    std::setw( width ) << std::setfill( ' ' ) << std::hex << std::left

//-------------------------------------------------------------------------------------
XpuL1Library::XpuL1Library() {

    if(!reader.load("libxpu.so") ) {
        printf( "File [libxpu.so] is not found!\n");
        exit(1);
    } else {
        loadFunctions();
    }
/*
    dump::header( std::cout, reader );
    dump::section_headers( std::cout, reader );
    dump::segment_headers( std::cout, reader );
    dump::symbol_tables( std::cout, reader );
    dump::notes( std::cout, reader );
    dump::modinfo( std::cout, reader );
    dump::dynamic_tags( std::cout, reader );
    dump::section_datas( std::cout, reader );
    dump::segment_datas( std::cout, reader );
*/

}

//-------------------------------------------------------------------------------------
void XpuL1Library::loadFunctions() {
    for ( const auto& sec : reader.sections ) { // For all sections
        if ( SHT_SYMTAB == sec->get_type() ||
             SHT_DYNSYM == sec->get_type() ) {
            const_symbol_section_accessor symbols( reader, sec.get() );

            Elf_Xword sym_no = symbols.get_symbols_num();
            if ( sym_no == 0 ) {
                continue;
            }

            std::cout << "Symbol table (" << sec->get_name() << ")" << std::endl;
            if ( reader.get_class() == ELFCLASS32 ) { // Output for 32-bit
                std::cout << "[  Nr ] Value      Size       Type    Bind     "
                       " Sect Name"
                    << std::endl;
            }
            else { // Output for 64-bit
                std::cout << "[  Nr ] Value              Size               "
                       "Type    Bind      Sect"
                    << std::endl
                    << "        Name" << std::endl;
            }
            for ( Elf_Xword i = 0; i < sym_no; ++i ) {
                std::string   name;
                Elf64_Addr    value   = 0;
                Elf_Xword     size    = 0;
                unsigned char bind    = 0;
                unsigned char type    = 0;
                Elf_Half      section = 0;
                unsigned char other   = 0;
                symbols.get_symbol( i, name, value, size, bind, type,
                                    section, other );
                loadFunction(i, name, value, size, bind, type,
                              section, reader.get_class() );
            }

            std::cout << std::endl;
        }
    }
}

//-------------------------------------------------------------------------------------
void XpuL1Library::loadFunction(Elf_Xword          no,
                              const std::string& name,
                              Elf64_Addr         value,
                              Elf_Xword          size,
                              unsigned char      bind,
                              unsigned char      type,
                              Elf_Half           section,
                              unsigned int       elf_class ) {
//        std::ios_base::fmtflags original_flags = out.flags();

        if ( elf_class == ELFCLASS32 ) { // Output for 32-bit
            std::cout << "[" << DUMP_DEC_FORMAT( 5 ) << no << "] "
                << DUMP_HEX0x_FORMAT( 8 ) << value << " "
                << DUMP_HEX0x_FORMAT( 8 ) << size << " " << DUMP_STR_FORMAT( 7 )
                << str_symbol_type( type ) << " " << DUMP_STR_FORMAT( 8 )
                << str_symbol_bind( bind ) << " " << DUMP_DEC_FORMAT( 5 )
                << section << " " << DUMP_STR_FORMAT( 1 ) << name << " "
                << std::endl;
        }
        else { // Output for 64-bit
            std::cout << "[" << DUMP_DEC_FORMAT( 5 ) << no << "] "
                << DUMP_HEX0x_FORMAT( 16 ) << value << " "
                << DUMP_HEX0x_FORMAT( 16 ) << size << " "
                << DUMP_STR_FORMAT( 7 ) << str_symbol_type( type ) << " "
                << DUMP_STR_FORMAT( 8 ) << str_symbol_bind( bind ) << " "
                << DUMP_DEC_FORMAT( 5 ) << section << " " << std::endl
                << "        " << DUMP_STR_FORMAT( 1 ) << name << " "
                << std::endl;
        }

//        out.flags( original_flags );
    }

//-------------------------------------------------------------------------------------
void XpuL1Library::writeFunction(std::string _name) {

}

//-------------------------------------------------------------------------------------
void XpuL1Library::writeData(void* _address, uint32_t _length){

}

//-------------------------------------------------------------------------------------
