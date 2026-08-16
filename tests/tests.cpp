#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <sstream>
#include <utilities.hpp>

using namespace std::literals;


TEST_CASE("Reading files", "[utilities][library]")
{
  SECTION("When readStream is called on an input stream, it returns the characters found on the stream")
  {
    std::istringstream iss;

    auto expectedResult = GENERATE("", "Hello, world!");
    iss.str(expectedResult);
    auto result = utilities::readStream(iss);

    REQUIRE(result == expectedResult);
  }

  SECTION("When readStream is called on an input stream, subsequent reads return the empty string")
  {
    std::istringstream iss;

    auto expectedResult = GENERATE("", "Hello, world!");
    iss.str(expectedResult);
    auto result = utilities::readStream(iss);
    result = utilities::readStream(iss);

    REQUIRE(result == "");
  }
}
