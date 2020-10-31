macro(run_conan)
# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(
    STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
       "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_add_remote(NAME bincrafters URL
                 https://api.bintray.com/conan/bincrafters/public-conan)

# The following is set in ~/.conan/profiles/default
#    compiler=gcc
#    compiler.version=10
#

conan_cmake_run(
  REQUIRES
  ${CONAN_EXTRA_REQUIRES}
  catch2/2.13.2
  docopt.cpp/0.6.2
  fmt/7.0.3
  spdlog/1.8.0
  range-v3/0.11.0
  tbb/2020.1
  OPTIONS
  ${CONAN_EXTRA_OPTIONS}
  SETTINGS compiler.cppstd=20 
  BASIC_SETUP
  CMAKE_TARGETS # individual targets to link to
  BUILD
  missing)
endmacro()
