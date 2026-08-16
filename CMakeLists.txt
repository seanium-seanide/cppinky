cmake_minimum_required(VERSION 3.25)
project(cppinky VERSION 1.0.0 LANGUAGES CXX)

include(FetchDependencies.cmake)

set(CPPINKY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(source)

option(BUILD_TESTING "Build and enable unit tests" ON)
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
