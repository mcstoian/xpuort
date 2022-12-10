#export LD_LIBRARY_PATH=/usr/local/bin/:/Users/marius/projects/protobuf/bazel-out/darwin_arm64-fastbuild/bin/:$LD_LIBRARY_PATH
#export LD_LIBRARY_PATH=/Users/marius/projects/protobuf/bazel-out/darwin_arm64-fastbuild/bin/_objs/protobuf/:$LD_LIBRARY_PATH
rm -rf build
mkdir build
cd build
export ONNX_ROOT=${GIT_ROOT}/onnx/install/
cmake -DONNX_ROOT=${GIT_ROOT}/onnx/install/ -DELFIO_ROOT=${GIT_ROOT}/ELFIO/ ..
make -j$(nproc)
