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
    std::ifstream _file;
    std::cout << "Loading lib/libxpu.json ..." << std::endl;
    _file.open("lib/libxpu.json");
    if(!_file) {
        printf("Failed to load libxpu!\n");
        exit(1);
    }
    libxpu = json::parse(_file);
    loadFunctions();
    xpuL4Driver->loadCode(getFunction());
}

//-------------------------------------------------------------------------------------
void XpuL3Library::loadFunctions() {
//    std::cout << libxpu.dump() << std::endl;
    for (json::iterator _it = libxpu.begin(); _it != libxpu.end(); ++_it) {
//        std::cout << *_it << '\n';
        loadFunction(_it);
    }

}

//-------------------------------------------------------------------------------------
void XpuL3Library::loadFunction(json::iterator _it) {
//        std::ios_base::fmtflags original_flags = out.flags();
//        std::cout << "[" << name << "]: " << DUMP_HEX0x_FORMAT( 16 ) << value << " : "<< DUMP_HEX0x_FORMAT( 16 ) << size << std::endl;
//        std::cout << "[" << _it.value() << "]" <<  std::endl;

        struct FunctionInfo _functionInfo;
        for (auto& _data : _it.value().items()){
//            std::cout << _data.value() << '\n';
            std::string _line = _data.value();
            uint32_t _byteCount = stoi(_line.substr(1, 3));
            uint32_t _address = stoi(_line.substr(3, 7));
            std::vector<uint8_t> _code;
            std::string::iterator _it = _line.begin();
            std::advance(_it, 8);
            _code.assign(_it, _line.end());
            struct FunctionCode _functionCode = {_byteCount, _address, _code};
            _functionInfo.codes.push_back(_functionCode);
        }
        std::pair<std::string, FunctionInfo> _functionEntry = {_it.key(), _functionInfo};
        functionMap.insert(_functionEntry);
    }

//-------------------------------------------------------------------------------------
FunctionInfo* XpuL3Library::getFunction(std::string _name) {
    std::unordered_map<std::string, FunctionInfo>::const_iterator _iterator = functionMap.find(_name);
    if(_iterator == functionMap.end()){
        return NULL;
    } else {
        return (FunctionInfo*)(&_iterator -> second);
    }
}


//-------------------------------------------------------------------------------------
void XpuL3Library::writeFunction(FunctionInfo* _functionInfo) {
//    std::cout << "Loading " << _name << std::endl;
/*        uint32_t _size = _iterator -> second.byteCount;
        std::vector<uint32_t> _data = _iterator -> second.data;
        for(int i = 0; i < _size; i++){
            uint32_t _dataWord = _data[i];
            xpuL4Driver -> AXI_LITE_write(0, _dataWord);
        }        */
}

//-------------------------------------------------------------------------------------
void XpuL3Library::writeData(void* _address, uint32_t _length){

}

//-------------------------------------------------------------------------------------
