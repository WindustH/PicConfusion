cmake_minimum_required(VERSION 3.30)
project(PicConfusion)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(OpenCV_DIR "H:/Environment/opencv/sources/build")
# set(FREEIMAGE_ROOT "H:/Environment/FreeImage")
# set(FREEIMAGE_INCLUDE_DIR "${FREEIMAGE_ROOT}/Source")
# set(FREEIMAGE_LIBRARY_DIR "${FREEIMAGE_ROOT}/Dist/x64")

add_executable(${PROJECT_NAME}
    src/main.cpp 
    src/codec.cpp
)

find_package(OpenCV REQUIRED)
find_package(OpenSSL REQUIRED)
# find_library(FREEIMAGE_LIBRARY
#     NAMES FreeImage freeimage
#     PATHS ${FREEIMAGE_LIBRARY_DIR}
#     REQUIRED
# )

target_include_directories(${PROJECT_NAME} PRIVATE 
"${CMAKE_SOURCE_DIR}/include"
    ${OpenCV_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    # ${FREEIMAGE_INCLUDE_DIR}

)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${OpenCV_LIBS}
    # ${FREEIMAGE_LIBRARY}
    ${OPENSSL_LIBRARIES}
)
