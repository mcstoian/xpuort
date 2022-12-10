//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <onnx/onnx_pb.h>
#include <XpuL2Resolver.h>

//-------------------------------------------------------------------------------------

class XpuL1OnnxRuntime {

public:
  XpuL1OnnxRuntime(onnx::GraphProto _graph);

  ~XpuL1OnnxRuntime();

  void process();

  private:
    onnx::GraphProto graph;
    XpuL2Resolver *xpuL2Resolver;
};
//-------------------------------------------------------------------------------------
