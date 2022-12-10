//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <onnx/defs/schema.h>
#include <onnx/defs/shape_inference.h>
#include <onnx/onnx_pb.h>
#include <onnx/defs/parser.h>
#include <onnx/shape_inference/implementation.h>
#include <XpuL1OnnxRuntime.h>

//-------------------------------------------------------------------------------------

class XpuL0OnnxGraph {

public:
  XpuL0OnnxGraph(std::string _filename);

  ~XpuL0OnnxGraph();

  void load(std::string _filename);
  void process();

  private:
    onnx::GraphProto graph;
    XpuL1OnnxRuntime *xpuL1OnnxRuntime;
};
//-------------------------------------------------------------------------------------
