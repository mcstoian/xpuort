cmake_minimum_required(VERSION 3.0.0)
project(xpuort VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-pointer-arith")

include(CTest)
enable_testing()

set(ONNX_NAMESPACE "onnx")
set(ONNX_ROOT ${ONNX_ROOT})
set(PROTOBUF_ROOT "/usr/")

find_library(PROTOBUF_LIBRARIES
    NAMES protobuf
    HINTS ${PROTOBUF_ROOT}/lib
)

find_path(PROTOBUF_INCLUDE_DIR
    NAMES google/protobuf/api.pb.h
    HINTS ${PROTOBUF_ROOT}/include
)

foreach(libname onnx onnx_proto)
    unset(libpath CACHE)
    find_library(ONNX_LIBRARIE_${libname}
        NAMES ${libname}
        HINTS ${ONNX_ROOT}/lib)
    list(APPEND ONNX_LIBRARIES ${ONNX_LIBRARIE_${libname}})
endforeach()

find_path(ONNX_INCLUDE_DIR
    NAMES onnx/onnx.pb.h
    HINTS ${ONNX_ROOT}/include
)

add_executable(xpuort
    main.cpp
    xpu/XpuDriver.cpp
    xpu/XpuOrt.cpp
)

target_compile_definitions(xpuort PRIVATE ONNX_NAMESPACE=${ONNX_NAMESPACE})

target_include_directories(xpuort PUBLIC
    ${PROTOBUF_INCLUDE_DIR}
    ${ONNX_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(xpuort PUBLIC
    ${PROTOBUF_LIBRARIES}
    ${ONNX_LIBRARIES}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
