add_executable(cppinky_tests)

target_sources(cppinky_tests
  PRIVATE
    tests.cpp
)

target_link_libraries(cppinky_tests
  PRIVATE
    Catch2::Catch2WithMain
    utilities_lib
)

target_compile_features(cppinky_tests
  PRIVATE
    cxx_std_23
)

target_compile_options(cppinky_tests
  PRIVATE
    $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang>:
      -Wall
      -Wextra
      -pedantic
      -Werror
      -Wno-missing-field-initializers
    >
    $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:
     /W4
     /WX
    >
)

add_test(
  NAME    cppinky_tests
  COMMAND cppinky_tests
)
