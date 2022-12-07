#export LD_LIBRARY_PATH=/usr/local/bin/:/Users/marius/projects/protobuf/bazel-out/darwin_arm64-fastbuild/bin/:$LD_LIBRARY_PATH
#export LD_LIBRARY_PATH=/Users/marius/projects/protobuf/bazel-out/darwin_arm64-fastbuild/bin/_objs/protobuf/:$LD_LIBRARY_PATH
rm -rf build
mkdir build
cd build
cmake -DONNX_ROOT=/home/petalinux/Gitroot/onnx/install -DELFIO_ROOT=/home/petalinux/Gitroot/ELFIO/ ..
make -j$(nproc)
