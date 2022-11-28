cmake_minimum_required(VERSION 3.0.0)
project(xpuort VERSION 0.1.0)

include(CTest)
enable_testing()
set(CMAKE_CXX_FLAGS "-DONNX_ML=1 -DONNX_NAMESPACE=onnx -std=c++17 -I/Users/marius/codex/nn/protobuf/src/")
set(ONNX_SRC_DIR "/Users/marius/codex/nn/onnx")
set(ONNX_BUILD_DIR "/Users/marius/codex/nn/onnx/build")
include_directories(${ONNX_BUILD_DIR})
include_directories(${ONNX_SRC_DIR})

add_executable(xpuort main.cpp)
target_link_libraries(xpuort PUBLIC ${ONNX_BUILD_DIR}/libonnx_proto.dylib ${ONNX_BUILD_DIR}/libonnx.dylib -lprotobuf)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
