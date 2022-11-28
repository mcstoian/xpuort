# Build ONNX from Source
```
git clone https://github.com/onnx/onnx.git
cd onnx
mkdir build
cd build
export ONNX_ROOT=<path-to-onnx>
export TENSOR_HDG_ROOT=<path-to-hdgbackend>
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DONNX_NAMESPACE=onnx -DCMAKE_BUILD_TYPE=Release -DONNX_ML=1 -DBUILD_SHARED_LIBS=ON 
-DONNX_USE_PROTOBUF_SHARED_LIBS=ON ../
make -j$(nproc)
  ```
  
# Build xpuort
```
gh repo clone mcstoian/xpuort
cd xpuort
mkdir build
cd build
cmake ../
make -j$(nproc)
./xpuort
```
