//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <XpuL3Library.h>

//-------------------------------------------------------------------------------------
XpuL3Library::XpuL3Library() {
    xpuL4Driver = new XpuL4Driver();
    std::ifstream f("./lib/libxpu.json");
    json libxpu = json::parse(f);
    loadFunctions();
}

//-------------------------------------------------------------------------------------
void XpuL3Library::loadFunctions() {
    std::cout << libxpu.dump() << std::endl;
    for (json::iterator _it = libxpu.begin(); _it != libxpu.end(); ++_it) {
        std::cout << *_it << '\n';
    }

}

//-------------------------------------------------------------------------------------
void XpuL3Library::loadFunction() {
//        std::ios_base::fmtflags original_flags = out.flags();
//        std::cout << "[" << name << "]: " << DUMP_HEX0x_FORMAT( 16 ) << value << " : "<< DUMP_HEX0x_FORMAT( 16 ) << size << std::endl;
//        functionMap.insert({ name, { value, size } });
    }

//-------------------------------------------------------------------------------------
void XpuL3Library::writeFunction(std::string _name) {
    std::cout << "Loading " << _name << std::endl;
    std::unordered_map<std::string, FunctionInfo>::const_iterator _iterator = functionMap.find(_name);
    if(_iterator == functionMap.end()){
        std::cout << "Could not find [" << _name << "] in library" << std::endl;
        exit(1);
    } else {
//        std::string _name = _iterator->second.name;
        uint32_t _size = _iterator -> second.size;
        uint32_t* _data = _iterator -> second.data;
        for(int i = 0; i < _size; i++){
            uint32_t _dataWord = *reinterpret_cast<int *>(_data + (8 * i));
            xpuL4Driver -> AXI_LITE_write(0, _dataWord);
        }        
    }
}

//-------------------------------------------------------------------------------------
void XpuL3Library::writeData(void* _address, uint32_t _length){

}

//-------------------------------------------------------------------------------------
