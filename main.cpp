//-------------------------------------------------------------------------------------
#include <fstream>
#include <iostream>
#include <vector>

#include <onnx/defs/schema.h>
#include <onnx/defs/shape_inference.h>
#include <onnx/onnx_pb.h>
#include <onnx/defs/parser.h>
#include <onnx/shape_inference/implementation.h>

#include <xpu/XpuOrt.h>
#include <xpu/XpuDriver.h>

//-------------------------------------------------------------------------------------
int main(int _argc, char* _argv[]) { 
  if (_argc != 2) {
    std::cout << "usage: xpuort model.onnx" << std::endl;
    return 1;
  }

  std::ifstream input(_argv[1], std::ios::ate | std::ios::binary); // open file and move current
                                             // position in file to the end

  std::streamsize size = input.tellg(); // get current position in file
  input.seekg(0, std::ios::beg);        // move to start of file
  std::vector<char> buffer(size);
  input.read(buffer.data(), size); // read raw data

  onnx::ModelProto model;
  model.ParseFromArray(buffer.data(), size); // parse protobuf

  ONNX_NAMESPACE::shape_inference::InferShapes(model);
  onnx::GraphProto graph = model.graph();

  XpuOrt* _xpuort = new XpuOrt(graph);
  _xpuort -> process();

  return 0;
}
//-------------------------------------------------------------------------------------
