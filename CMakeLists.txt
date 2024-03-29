cmake_minimum_required(VERSION 3.16)

set(GRAPH_VERSION 0.1.0)

# Set the project name to your project name, my project isn't very descriptive
project(graph VERSION ${GRAPH_VERSION} LANGUAGES CXX)
include(cmake/StandardProjectSettings.cmake)

add_library(graph INTERFACE)
target_include_directories(graph INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(graph INTERFACE CONAN_PKG::range-v3)

add_library(graph_adaptor INTERFACE)
target_include_directories(graph_adaptor INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/adaptor/")

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
#target_compile_options(project_options INTERFACE $<$<CXX_COMPILER_ID:Clang>:>)
target_compile_options(project_options INTERFACE $<$<CXX_COMPILER_ID:GNU>:--concepts>)
target_compile_options(project_options INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(project_options INTERFACE -ftime-trace)
  endif()
endif()
 
# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# Add linker configuration
include(cmake/Linker.cmake)
configure_linker(project_options)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)
option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's change
  # 
  # consider breaking this out per project as necessary 
  target_precompile_headers(
    project_options
    INTERFACE
    <vector>
    <string>
    <map>
    <utility>)
endif()

option(ENABLE_BGL17 "Enable Better Graph Library 17" OFF)
if(ENABLE_BGL17)
endif()

# Set up some extra Conan dependencies based on our needs before loading Conan
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")


include(cmake/Conan.cmake)
run_conan()

if(ENABLE_TESTING)
  enable_testing()
  message("Building Tests. Be sure to check out test/constexpr_tests for constexpr testing")
  add_subdirectory(test)
endif()

if(ENABLE_FUZZING)
  message("Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
  add_subdirectory(fuzz_test)
endif()


#add_subdirectory(src)

