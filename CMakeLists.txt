cmake_minimum_required(VERSION 3.30)
project(PicConfusion)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(OpenCV_DIR "H:/Environment/opencv/sources/build")
include_directories("${CMAKE_SOURCE_DIR}/include")

# 查找OpenCV包
find_package(OpenCV REQUIRED)

# 包含OpenCV头文件目录
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(PicConfusion
    src/main.cpp 
    src/codec.cpp
    )

target_link_libraries(PicConfusion ${OpenCV_LIBS})
find_package(OpenSSL REQUIRED)

# 包含头文件路径
include_directories(${OPENSSL_INCLUDE_DIR})

# 链接OpenSSL库
target_link_libraries(PicConfusion ${OPENSSL_LIBRARIES})