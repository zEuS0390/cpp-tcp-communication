cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(tcp-socket LANGUAGES CXX VERSION 1.0)
add_executable(tcp-socket src/main.cpp src/options.cpp)

if (WIN32)
    target_link_libraries(tcp-socket ws2_32)
endif ()

target_include_directories(tcp-socket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
