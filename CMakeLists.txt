cmake_minimum_required(VERSION 2.6)

project("RuSseL" LANGUAGES C CXX)

set(VERBOSE FALSE)
set(BUILD_TYPE "Debug")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./run)
add_executable(russel)

# The exact path of all requisite header files must be specified below
target_include_directories(russel PUBLIC
  ./src/include
  /usr/lib/gcc/x86_64-linux-gnu/9/include
  /home/cjrevelas/libraries/mumps_serial_cpp/PORD/include
  /home/cjrevelas/libraries/mumps_serial_cpp/libseq
  /home/cjrevelas/libraries/mumps_serial_cpp/include
)

# The exact path of all requisite libraries must be specified below
set(CMAKE_PREFIX_PATH "/usr/lib/gcc/x86_64-linux-gnu/9/;"
                      "/home/cjrevelas/libraries/lapack-3.9.0/;"
                      "/home/cjrevelas/libraries/mumps_serial_cpp/lib/;"
                      "/home/cjrevelas/libraries/mumps_serial_cpp/libseq;"
                      "/home/cjrevelas/libraries/mumps_serial_cpp/PORD/lib"
)

find_library(LAPACK_LIB lapack)
find_library(BLAS_LIB refblas)
find_library(PORD_LIB pord)
find_library(MPISEQ_LIB mpiseq)
find_library(DMUMPS_LIB dmumps)
find_library(MUMPS_LIB  mumps_common)
find_library(GFORTRAN_LIB gfortran)
find_library(PTHREAD_LIB pthread)

if (VERBOSE)
  message("CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
    
  message(${LAPACK_LIB})
  message(${BLAS_LIB})
  message(${PORD_LIB})
  message(${MPISEQ_LIB})
  message(${DMUMPS_LIB})
  message(${MUMPS_LIB})
  message(${GFORTRAN_LIB})
  message(${PTHREAD_LIB})

  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("Building project with GNU compiler")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    message("Building project with Intel compiler")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "CLANG")
    message("Building project with CLANG compiler")
  endif()
endif()

set(CMAKE_BUILD_TYPE ${BUILD_TYPE} CACHE STRING "debug_compiler_configuration" FORCE)

# Configuring compilation with GNU compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(gcc_debug_flags "-O0 -g -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG ${gcc_debug_flags} CACHE STRING "gcc_debug_flags" FORCE)
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(gcc_release_flags "-O3 -m64 -Wall -mtune=native -march=native -Wno-sign-compare")
    set(CMAKE_CXX_FLAGS_RELEASE ${gcc_release_flags} CACHE STRING "gcc_release_flags" FORCE)
  endif()
endif()

# Configuring compilation with Intel compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(intel_debug_flags "-O0 -g -traceback -debug all")
    set(CMAKE_CXX_FLAGS_DEBUG ${intel_debug_flags} CACHE STRING "intel_debug_flags" FORCE)
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(intel_release_flags "-O3 -xHost -align -ip -ipo -prec-div -prec-sqrt")
    set(CMAKE_CXX_FLAGS_RELEASE ${intel_release_flags} CACHE STRING "intel_release_flags" FORCE)
  endif()
endif()

target_sources(russel PUBLIC
  src/Eos.cpp
  src/Fem.cpp
  src/Main.cpp
  src/Mesh.cpp
  src/Memory.cpp
  src/Parser.cpp
  src/Russel.cpp
  src/EvalArg.cpp
  src/IOHelper.cpp
  src/Variable.cpp
  src/Evaluator.cpp
  src/EosHelfand.cpp
  src/EosSanchezLacombe.cpp
  src/StringOperations.cpp
)

target_link_libraries(russel PUBLIC 
  ${DMUMPS_LIB}
  ${MUMPS_LIB}
  ${PORD_LIB}
  ${LAPACK_LIB}
  ${MPISEQ_LIB}
  ${PTHREAD_LIB}
  ${BLAS_LIB}
  ${GFORTRAN_LIB}
)

set_target_properties(russel PROPERTIES LINKER_LANGUAGE CXX)
