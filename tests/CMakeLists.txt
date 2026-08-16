set(CPPINKY_TESTS cppinky_tests)

add_executable(${CPPINKY_TESTS})

target_sources(${CPPINKY_TESTS}
  PRIVATE
    tests.cpp
)

target_link_libraries(${CPPINKY_TESTS}
  PRIVATE
    Catch2::Catch2WithMain
    ${CPPINKY_LIB}
)

target_compile_features(${CPPINKY_TESTS}
  PRIVATE
    cxx_std_23
)

target_compile_options(${CPPINKY_TESTS}
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
  NAME    ${CPPINKY_TESTS}
  COMMAND ${CPPINKY_TESTS}
)
