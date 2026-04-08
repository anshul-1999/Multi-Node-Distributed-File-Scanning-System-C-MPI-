cmake_minimum_required(VERSION 3.20)
project(Parallel_Computing CXX)

set(CMAKE_CXX_STANDARD 17)

# 1. Find OpenMP
find_package(OpenMP REQUIRED)

# 2. Sequential Version
add_executable(sequential_scanner sequential_scanner.cpp)

# 3. Parallel Version (OpenMP)
add_executable(parallel_scanner parallel_scanner.cpp)

# 4. Link OpenMP
if(OpenMP_CXX_FOUND)
    target_link_libraries(parallel_scanner PUBLIC OpenMP::OpenMP_CXX)
else()
    message(FATAL_ERROR "OpenMP not found! Ensure your compiler supports -fopenmp")
endif()