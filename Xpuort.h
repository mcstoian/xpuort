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
  Xpuort(auto _graph);

  ~Xpuort();

  int process();

  private:
    auto graph;
};

//-------------------------------------------------------------------------------------
