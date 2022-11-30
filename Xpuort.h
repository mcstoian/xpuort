//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
class Xpuort {

public:
  Xpuort(Onnx::GraphProto _graph);

  ~Xpuort();

  int process();

  private:
    Onnx::GraphProto graph;
};

//-------------------------------------------------------------------------------------
