#option(CPP_STARTER_USE_IMGUI "Enable compilation of ImGui sample" OFF)

# imgui example
#if(CPP_STARTER_USE_IMGUI)
#  message("Using Dear ImGui")
#  add_subdirectory(imgui)
#endif()

# Generic test that uses conan libs
add_executable(intro main.cpp)
target_link_libraries(
  intro PRIVATE project_options project_warnings CONAN_PKG::docopt.cpp
                CONAN_PKG::fmt CONAN_PKG::spdlog)
