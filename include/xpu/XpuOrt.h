#pragma once

//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------

#include <onnx/onnx_pb.h>
#include <XpuDriver.h>

class XpuOrt {

public:
  XpuOrt(onnx::GraphProto _graph);

  ~XpuOrt();

  void process();

  private:
    onnx::GraphProto graph;
    XpuDriver *xpuDriver;
};

//-------------------------------------------------------------------------------------
