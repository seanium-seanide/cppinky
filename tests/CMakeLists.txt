# utilities tests

add_executable(utilities_tests)

target_sources(utilities_tests
  PRIVATE
    utilities.cpp
)

target_link_libraries(utilities_tests
  PRIVATE
    Catch2::Catch2WithMain
    utilities_lib
)

target_compile_features(utilities_tests
  PRIVATE
    cxx_std_26
)

target_compile_options(utilities_tests
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
  NAME    utilities_tests
  COMMAND utilities_tests
)

# tokenizer tests

add_executable(tokenizer_tests)

target_sources(tokenizer_tests
  PRIVATE
    tokenizer.cpp
)

target_link_libraries(tokenizer_tests
  PRIVATE
    Catch2::Catch2WithMain
    cppinky_lib
)

target_compile_features(tokenizer_tests
  PRIVATE
    cxx_std_26
)

target_compile_options(tokenizer_tests
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
  NAME    tokenizer_tests
  COMMAND tokenizer_tests
)
