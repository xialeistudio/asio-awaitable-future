cmake_minimum_required(VERSION 3.15)
project(asio-future VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Find asio headers manually
find_path(ASIO_INCLUDE_DIR 
    NAMES asio.hpp
)

if(NOT ASIO_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find asio.hpp. Please install asio or set ASIO_INCLUDE_DIR")
endif()

message(STATUS "Found asio headers at: ${ASIO_INCLUDE_DIR}")

# Create interface library for header-only library
add_library(asio-future INTERFACE)
add_library(asio-future::asio-future ALIAS asio-future)

# Set include directories
target_include_directories(asio-future INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ASIO_INCLUDE_DIR}
)

# Link dependencies
target_link_libraries(asio-future INTERFACE)

# Set compile features and definitions
target_compile_features(asio-future INTERFACE cxx_std_20)
target_compile_definitions(asio-future INTERFACE 
    ASIO_STANDALONE
    ASIO_NO_DEPRECATED
)

# Build examples
option(BUILD_EXAMPLES "Build example programs" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif() 