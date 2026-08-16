include(FetchContent)

FetchContent_Declare(fmt
  URL       https://github.com/fmtlib/fmt/releases/download/12.2.0/fmt-12.2.0.zip
  URL_HASH  SHA256=a2f4a8d51178f954e4c339007f77edd76ba0cb2e36f87a48e5a5403d9be5878f
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(Catch2
  URL       https://github.com/catchorg/Catch2/archive/refs/tags/v3.15.3.tar.gz
  URL_HASH  SHA256=b0299ae552918220a7a6e21e7de5b714777f4e8c883fb70c4bb23fe01df8c6e3
)
FetchContent_MakeAvailable(Catch2)

