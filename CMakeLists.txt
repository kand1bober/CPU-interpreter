cmake_minimum_required(VERSION 3.30)
set(project_name cpu)

project(${project_name})

#libraries
file(GLOB srcs_lib "utilities/*.cpp")
file(GLOB hdrs_lib "utilities/*.hpp")
add_library(utilities_lib STATIC ${srcs_lib} ${hdrs_lib})

set(ASMJIT_DIR "3rdparty/asmjit")              # Location of AsmJit.
set(ASMJIT_STATIC TRUE)                        # Force static build.
add_subdirectory("${ASMJIT_DIR}")              # Adds AsmJit sub-project to your project.

#main part
file(GLOB srcs "cpu/src/*.cpp")
file(GLOB hdrs "cpu/include/*.cpp")
add_executable(${project_name} ${srcs} ${hdrs})
target_link_libraries(${project_name} PUBLIC utilities_lib asmjit::asmjit) # Adds AsmJit as a dependency to app.
target_compile_features(${project_name} PUBLIC cxx_std_17) # Makes C++17 as a requirement.