//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <XpuL1OnnxRuntime.h>

//-------------------------------------------------------------------------------------

class XpuL0OnnxGraph {

public:
  XpuL0OnnxGraph(str::string _filename);

  ~XpuL0OnnxGraph();

  void load();

  private:
    XpuL1OnnxRuntime *xpuL1OnnxRuntime;
};
//-------------------------------------------------------------------------------------
