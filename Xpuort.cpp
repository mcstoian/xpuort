//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
Xpuort::Xpuort(onnx::GraphProto _graph) {
  graph = _graph;
}

Xpuort::~Xpuort() {
}

void Xpuort::process() {

  for (size_t i = 0; i < graph.value_info_size(); i++) {
    const onnx::ValueInfoProto info = graph.value_info(i);
    std::string name = info.name();
    auto shape = info.type().tensor_type().shape();
    if (shape.dim_size() > 0) {
      int size = shape.dim_size();
      std::cout << name << " : " << shape.dim(0).dim_param();
      for (int i = 1; i < size; ++i) {
        std::cout << ", " << shape.dim(i).dim_value();
      }
      std::cout << std::endl;
    }
  }

}
//-------------------------------------------------------------------------------------
