//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <XpuL0OnnxGraph.h>

//-------------------------------------------------------------------------------------
int main(int _argc, char* _argv[]) { 
  std::cout << "Xpuort v0.1..." << std::endl;
  if(_argc != 2) {
    std::cout << "usage: xpuort model.onnx" << std::endl;
    return 1;
  }

  XpuL0OnnxGraph* _xpuL1OnnxGraph = new XpuL0OnnxGraph(_argv[1]);
  return 0;
}
//-------------------------------------------------------------------------------------
