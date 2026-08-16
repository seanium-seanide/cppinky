# cppinky_lib

set(CPPINKY_LIB cppinky_lib)

add_library(${CPPINKY_LIB})

target_sources(${CPPINKY_LIB}
  PRIVATE
    Interpreter.cpp

  PUBLIC FILE_SET HEADERS
    BASE_DIRS
      ${CPPINKY_INCLUDE_DIR}
    FILES
      ${CPPINKY_INCLUDE_DIR}/Interpreter.hpp
)

target_link_libraries(${CPPINKY_LIB}
  PRIVATE
    fmt::fmt
)

target_compile_features(${CPPINKY_LIB}
  PRIVATE
    cxx_std_23
)

target_compile_options(${CPPINKY_LIB}
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

add_executable(${CPPINKY_EXE})

target_sources(${CPPINKY_EXE}
  PRIVATE
    main.cpp
)

target_link_libraries(${CPPINKY_EXE}
  PRIVATE
    ${CPPINKY_LIB}
)

target_compile_features(${CPPINKY_EXE}
  PRIVATE
    cxx_std_23
)

target_compile_options(${CPPINKY_EXE}
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
