//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <XpuL1OnnxRuntime.h>

//-------------------------------------------------------------------------------------
XpuL0OnnxGraph::XpuL0OnnxGraph(std::string _filename) {

  std::ifstream input(_filename, std::ios::ate | std::ios::binary); // open file and move current
                                             // position in file to the end

  std::streamsize size = input.tellg(); // get current position in file
  input.seekg(0, std::ios::beg);        // move to start of file
  std::vector<char> buffer(size);
  input.read(buffer.data(), size); // read raw data

  onnx::ModelProto model;
  model.ParseFromArray(buffer.data(), size); // parse protobuf

  ONNX_NAMESPACE::shape_inference::InferShapes(model);
  onnx::GraphProto graph = model.graph();

  XpuL1OnnxRuntime* _xpuL0Onnx = new XpuL1OnnxRuntime(graph);
  _xpuL0Onnx -> process();
}

//-------------------------------------------------------------------------------------
XpuL0OnnxGraph::~XpuL0OnnxGraph() {
}

//-------------------------------------------------------------------------------------
void XpuL0OnnxGraph::process() {

  for (size_t i = 0; i < graph.value_info_size(); i++) {
    const onnx::ValueInfoProto info = graph.value_info(i);
    std::string name = info.name();
    auto shape = info.type().tensor_type().shape();
    if (shape.dim_size() > 0) {
      int size = shape.dim_size();
      std::cout << name << " : " << shape.dim(0).dim_param();
      XpuL3Library -> writeFunction(name);
      for (int i = 1; i < size; i++) {
        std::cout << ", " << shape.dim(i).dim_value();
//        xpuDriver -> writeData()
      }
      std::cout << std::endl;
    }
  }

}
//-------------------------------------------------------------------------------------
