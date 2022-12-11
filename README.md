# Build ONNX from Source
```
git clone https://github.com/onnx/onnx.git
cd onnx
mkdir build
cd build
export ONNX_ROOT=<onnx-installation-absolute-path>
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DONNX_NAMESPACE=onnx -DCMAKE_BUILD_TYPE=Release -DONNX_ML=0 -DBUILD_SHARED_LIBS=ON 
-DONNX_USE_PROTOBUF_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=${ONNX_ROOT} ../
make -j$(nproc) && make install
```
  
# Build xpuort
```
git clone https://github.com/mcstoian/xpuort.git
cd $GIT_ROOT/xpuort
./build.sh
```

# Run xpuort
```
cd $GIT_ROOT/xpuort
./run.sh
```
