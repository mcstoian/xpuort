//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <onnx/onnx_pb.h>
#include <XpuL1Library.h>

//-------------------------------------------------------------------------------------

class XpuL0Ort {

public:
  XpuL0Ort(onnx::GraphProto _graph);

  ~XpuL0Ort();

  void process();

  private:
    onnx::GraphProto graph;
    XpuL1Library *xpuL1Library;
};
//-------------------------------------------------------------------------------------
