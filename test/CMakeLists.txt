# automatically enable catch2 to generate ctest targets
if(CONAN_CATCH2_ROOT_DEBUG)
  include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else()
  include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif()


add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC CONAN_PKG::catch2)
target_link_libraries(catch_main PRIVATE project_options)

add_executable(tests tests.cpp test_dav_graph.cpp data_routes.cpp test_ual_graph.cpp test_concomp.cpp test_ordered_pair.cpp test_rl.cpp test_temp.cpp)
target_link_libraries(tests PRIVATE project_warnings project_options catch_main)
target_link_libraries(tests PRIVATE CONAN_PKG::fmt)
target_link_libraries(tests PRIVATE graph)
target_link_libraries(tests PRIVATE graph_adaptor)

if(ENABLE_BGL17)
  target_link_libraries(catch_main PUBLIC CONAN_PKG::tbb)

  target_include_directories(graph INTERFACE "${CMAKE_SOURCE_DIR}/../bgl17/src/")
  target_include_directories(graph_adaptor INTERFACE "${CMAKE_SOURCE_DIR}/../adaptor/")
  target_sources(tests PRIVATE test_vov_adaptor)
  #target_compile_options(tests PRIVATE -Wno-pedantic -Wno-shadow)
  #set(GCC_WARNINGS, ${GCC_WARNINGS} -Wno-pedantic -Wno-shadow)
endif()


# automatically discover tests that are defined in catch based test files you
# can modify the unittests. TEST_PREFIX to whatever you want, or use different
# for different binaries. See 
# https://github.com/catchorg/Catch2/commit/3d7104c124e3af88eda573f867f79d0aa9ef6c72
# for more info about catch_discover_tests.
catch_discover_tests(
  tests
  TEST_PREFIX
  "unittests."
  EXTRA_ARGS
  -s
  --reporter=xml
  --out=tests.xml)

# Add a file containing a set of constexpr tests
add_executable(constexpr_tests constexpr_tests.cpp)
target_link_libraries(constexpr_tests PRIVATE project_options project_warnings
                                              catch_main)

catch_discover_tests(
  constexpr_tests
  TEST_PREFIX
  "constexpr."
  EXTRA_ARGS
  -s
  --reporter=xml
  --out=constexpr.xml)

# Disable the constexpr portion of the test, and build again this allows us to
# have an executable that we can debug when things go wrong with the constexpr
# testing
add_executable(relaxed_constexpr_tests constexpr_tests.cpp)
target_link_libraries(
  relaxed_constexpr_tests PRIVATE project_options project_warnings
                                  catch_main)
target_compile_definitions(
  relaxed_constexpr_tests PRIVATE
                                  -DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE)


catch_discover_tests(
  relaxed_constexpr_tests
  TEST_PREFIX
  "relaxed_constexpr."
  EXTRA_ARGS
  -s
  --reporter=xml
  --out=relaxed_constexpr.xml)
