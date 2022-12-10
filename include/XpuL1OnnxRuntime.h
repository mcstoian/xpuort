//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <XpuL2Resolver.h>

//-------------------------------------------------------------------------------------

class XpuL1OnnxRuntime {

public:
  XpuL1OnnxRuntime();

  ~XpuL1OnnxRuntime();

  void run(std::string _name);

  private:
    XpuL2Resolver *xpuL2Resolver;
};
//-------------------------------------------------------------------------------------
