# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the C++ standard to C++20 and make it a required setting
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the directory for the compiled executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define the project, setting the name, language, and version
project(tcp-socket LANGUAGES CXX VERSION 1.0)

# Add an executable target called 'tcp-socket' from the specified source files
add_executable(tcp-socket src/main.cpp src/options.cpp)

# Conditional block for Windows-specific configurations
if (WIN32)
    # Link the 'ws2_32' library for Winsock (necessary for networking on Windows)
    target_link_libraries(tcp-socket ws2_32)
endif ()

# Specify the include directory for the project, making it accessible to the target
target_include_directories(tcp-socket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
