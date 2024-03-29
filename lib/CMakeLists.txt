cmake_minimum_required(VERSION 3.0.0)

# CUDA
find_package(CUDA REQUIRED)
include_directories(/usr/local/cuda/include)
link_directories(/usr/local/cuda/lib64)

# OpenCV
find_package(OpenCV 3.4.15 REQUIRED PATHS /root/PLATFORM/3rdparty/opencv3415)
# find_package(OpenCV REQUIRED)

# TensorRT
set(TENSORRT_ROOT /root/PLATFORM/3rdparty/TensorRT-7.0.0.11)
set(TENSORRT_INCLUDE_DIR ${TENSORRT_ROOT}/include)
set(TENSORRT_LIB_DIR ${TENSORRT_ROOT}/lib)

#libtorch
set(LIBTORCH_ROOT /root/PLATFORM/3rdparty/libtorch131)
set(LIBTORCH_INCLUDE_DIRS ${LIBTORCH_ROOT}/include ${LIBTORCH_ROOT}/include/torch/csrc/api/include)
set(LIBTORCH_LIB_DIR ${LIBTORCH_ROOT}/lib)

#
aux_source_directory(. LIB_SRC)
add_library(human3d SHARED ${LIB_SRC} )

target_include_directories(human3d 
            PUBLIC ${OpenCV_INCLUDE_DIRS}
            PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extra
            )


message(${LIBTORCH_LIB_DIR}/libtorch.so)
target_link_libraries(human3d ${OpenCV_LIBS} 
                      ${TENSORRT_LIB_DIR}/libnvinfer.so 
                      ${CUDA_LIBRARIES}
                      ${CMAKE_CURRENT_SOURCE_DIR}/extra/libdetector.so
                      ${CMAKE_CURRENT_SOURCE_DIR}/extra/libhmr.so 
                      ${CMAKE_CURRENT_SOURCE_DIR}/extra/libkeypoints.so 
                      ${CMAKE_CURRENT_SOURCE_DIR}/extra/libtracker.so 
                      ${LIBTORCH_LIB_DIR}/libtorch.so
                      )

# cd build 
# make install
install(FILES class_human3d.h DESTINATION include)
install(TARGETS human3d LIBRARY DESTINATION lib)
install(FILES extra/yolov5s_fp16_b1.engine DESTINATION lib)
install(FILES extra/hmr_fp16_b1.engine DESTINATION lib)
install(FILES extra/kp_fp16_b1.engine DESTINATION lib)
install(FILES extra/smpl_male.json DESTINATION lib)
install(FILES extra/libdetector.so DESTINATION lib)
install(FILES extra/libhmr.so DESTINATION lib)
install(FILES extra/libkeypoints.so DESTINATION lib)
install(FILES extra/libtracker.so DESTINATION lib)
install(FILES ../data/im1010.jpg DESTINATION .)
