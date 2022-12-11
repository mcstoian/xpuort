//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <XpuL3Library.h>

//-------------------------------------------------------------------------------------
class XpuL2Resolver {

public:
	XpuL2Resolver();

 	~XpuL2Resolver();

	void resolve(std::string _name);
	void uploadFunction(std::string _name);
	void uploadData(void* _address, uint32_t _length);

private:
	XpuL3Library* xpuL3Library;
	std::unordered_map<std::string, std::any> internallyResolvedFunctionMap;
};
//-------------------------------------------------------------------------------------






