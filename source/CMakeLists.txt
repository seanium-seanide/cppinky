# utilities_lib

add_library(utilities_lib)

target_sources(utilities_lib
  PRIVATE
    utilities.cpp

  PUBLIC FILE_SET HEADERS
    BASE_DIRS
      ${CPPINKY_INCLUDE_DIR}
    FILES
      ${CPPINKY_INCLUDE_DIR}/utilities.hpp
)

target_link_libraries(utilities_lib
  PRIVATE
    fmt::fmt
)

target_compile_features(utilities_lib
  PRIVATE
    cxx_std_23
)

target_compile_options(utilities_lib
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

# cppinky_lib

set(CPPINKY_LIB cppinky_lib)

add_library(cppinky_lib)

target_sources(cppinky_lib
  PRIVATE
    Interpreter.cpp
    Tokenizer.cpp
    Token.cpp

  PUBLIC FILE_SET HEADERS
    BASE_DIRS
      ${CPPINKY_INCLUDE_DIR}
    FILES
      ${CPPINKY_INCLUDE_DIR}/Interpreter.hpp
      ${CPPINKY_INCLUDE_DIR}/Tokenizer.hpp
      ${CPPINKY_INCLUDE_DIR}/Token.hpp
      ${CPPINKY_INCLUDE_DIR}/Token.tpp
)

target_link_libraries(cppinky_lib
  PRIVATE
    argparse::argparse
    utilities_lib

  PUBLIC
    fmt::fmt
    magic_enum
)

target_compile_features(cppinky_lib
  PRIVATE
    cxx_std_23
)

target_compile_options(cppinky_lib
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

# cppinky_exe

set(CPPINKY_EXE cppinky)

add_executable(cppinky)

target_sources(cppinky
  PRIVATE
    main.cpp
)

target_link_libraries(cppinky
  PRIVATE
    cppinky_lib
)

target_compile_features(cppinky
  PRIVATE
    cxx_std_23
)

target_compile_options(cppinky
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
