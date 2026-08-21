#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include <sstream>
#include <utilities.hpp>

using namespace std::literals;


TEST_CASE("Reading files", "[utilities]")
{
  SECTION("When readStream is called on an input stream, it returns the characters found on the stream.")
  {
    std::istringstream iss;

    auto expectedResult = GENERATE("", "Hello, world!");
    iss.str(expectedResult);
    auto result = utilities::readStream(iss);

    REQUIRE(result == expectedResult);
  }

  SECTION("When readStream is called on an input stream, subsequent reads return the empty string.")
  {
    std::istringstream iss;

    auto expectedResult = GENERATE("", "Hello, world!");
    iss.str(expectedResult);
    auto result = utilities::readStream(iss);
    result = utilities::readStream(iss);

    REQUIRE(result == "");
  }
}

TEST_CASE("Character predicates", "[utilities]")
{
  // alpha ::= 'a' | 'b' | ... | 'z' | ... | 'A' | 'B' | ... 'Z' | '_'
  // digit ::= '0' | '1' | ... | '9'

  SECTION("isdigit returns true when passed a digit character")
  {
    auto c = GENERATE(range<char>('0', '9' + 1));

    auto result = utilities::isdigit(c);

    REQUIRE(result == true);
  }

  SECTION("isdigit returns false when passed a non-digit character")
  {
    auto c = GENERATE(range<int>(0, '0'), range<int>('9' + 1, 256));

    auto result = utilities::isdigit(c);

    REQUIRE(result == false);
  }

  SECTION("isalpha returns true when passed an alpha character or an underscore")
  {
    auto c = GENERATE(range<char>('A', 'Z' + 1), range<char>('a', 'z' + 1), '_');

    auto result = utilities::isalpha(c);

    REQUIRE(result == true);
  }

  SECTION("isalpha returns false when passed a character that is neither alpha nor an underscore")
  {
    auto c = GENERATE(
      range<int>(0, 'A'), range<int>('Z' + 1, '_'), range<int>('_' + 1, 'a'), range<int>('z' + 1, 256)
    );

    auto result = utilities::isalpha(c);

    REQUIRE(result == false);
  }

  SECTION("isspace returns true when passed a whitespace character")
  {
    auto c = GENERATE(' ', '\f', '\n', '\r', '\t', '\v');

    auto result = utilities::isspace(c);

    REQUIRE(result == true);
  }

  SECTION("isspace returns false when passed a non-whitespace character")
  {
    auto c = GENERATE(
               filter(
                 [](int c)
                 {
                   return std::string_view{" \f\n\r\t\v"}.find(c) == std::string_view::npos;
                 }
               , range(0, 256)
               )
             );

    auto result = utilities::isspace(c);

    REQUIRE(result == false);
  }
}
