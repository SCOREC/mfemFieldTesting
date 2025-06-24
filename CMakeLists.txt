cmake_minimum_required(VERSION 3.12)
project(MFEM_GPU_Field_Eval LANGUAGES CUDA CXX)
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-extended-lambda")

# Find MFEM (assumes MFEM was installed with CMake)
find_package(MFEM REQUIRED CONFIG)

# Add the executable
add_executable(fieldEval fieldEval.cpp)
set_source_files_properties(fieldEval.cpp PROPERTIES LANGUAGE CUDA) 
target_link_libraries(fieldEval mfem)
