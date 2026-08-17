#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <sstream>
#include <vector>
#include <ranges>
#include <utilities.hpp>
#include <Tokenizer.hpp>

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


TEST_CASE("Tokenizing scripts", "[cppinky]")
{
  SECTION("When tokenizing an empty script, an empty sequence of tokens is returned.")
  {
    auto script = ""s;
    auto tokenizer = cppinky::Tokenizer(script);

    auto result = tokenizer.tokenize();

    REQUIRE(result.size() == 0);
  }

  SECTION("When tokenizing a script containing a single character, a single token is returned.")
  {
    auto [script, expectedTokenType] = GENERATE(
      std::pair("+", cppinky::TokenType::PLUS)
    , std::pair("-", cppinky::TokenType::MINUS)
    , std::pair("*", cppinky::TokenType::TIMES)
    , std::pair("/", cppinky::TokenType::DIVIDE)
    );
    auto tokenizer = cppinky::Tokenizer(script);

    auto result = tokenizer.tokenize();

    REQUIRE(result.size() == 1);
    REQUIRE(result.back().type == expectedTokenType);
    REQUIRE(result.back().lexeme == script);
  }

  SECTION("When tokenizing a script, the correct sequence of lexemes is returned")
  {
    auto [script, tokens] = GENERATE(
      std::pair("()[]{}.,+-*/"s, std::vector({
        cppinky::Token(cppinky::TokenType::LEFT_PAREN, "(")
      , cppinky::Token(cppinky::TokenType::RIGHT_PAREN, ")")
      , cppinky::Token(cppinky::TokenType::LEFT_SQUARE, "[")
      , cppinky::Token(cppinky::TokenType::RIGHT_SQUARE, "]")
      , cppinky::Token(cppinky::TokenType::LEFT_CURLY, "{")
      , cppinky::Token(cppinky::TokenType::RIGHT_CURLY, "}")
      , cppinky::Token(cppinky::TokenType::DOT, ".")
      , cppinky::Token(cppinky::TokenType::COMMA, ",")
      , cppinky::Token(cppinky::TokenType::PLUS, "+")
      , cppinky::Token(cppinky::TokenType::MINUS, "-")
      , cppinky::Token(cppinky::TokenType::TIMES, "*")
      , cppinky::Token(cppinky::TokenType::DIVIDE, "/")
      }))
    , std::pair("**-+-/-+**/", std::vector({
        cppinky::Token(cppinky::TokenType::TIMES, "*")
      , cppinky::Token(cppinky::TokenType::TIMES, "*")
      , cppinky::Token(cppinky::TokenType::MINUS, "-")
      , cppinky::Token(cppinky::TokenType::PLUS, "+")
      , cppinky::Token(cppinky::TokenType::MINUS, "-")
      , cppinky::Token(cppinky::TokenType::DIVIDE, "/")
      , cppinky::Token(cppinky::TokenType::MINUS, "-")
      , cppinky::Token(cppinky::TokenType::PLUS, "+")
      , cppinky::Token(cppinky::TokenType::TIMES, "*")
      , cppinky::Token(cppinky::TokenType::TIMES, "*")
      , cppinky::Token(cppinky::TokenType::DIVIDE, "/")
      }))
    );
    auto tokenizer = cppinky::Tokenizer(script);

    auto result = tokenizer.tokenize();
    REQUIRE(result.size() == tokens.size());

    REQUIRE(result.size() == script.size());
    for (auto [result, token]: std::views::zip(result, tokens))
    {
      REQUIRE(result.type == token.type);
      REQUIRE(result.lexeme == token.lexeme);
    }
  }

  SECTION("When the tokenizer encounters whitespace, a token is not emitted.")
  {
    auto [script, tokens] = GENERATE(
      std::pair("        +"s, std::vector({
        cppinky::Token(cppinky::TokenType::PLUS, "+")
      }))
    , std::pair("\n\n\n\n\n+"s, std::vector({
        cppinky::Token(cppinky::TokenType::PLUS, "+")
      }))
    , std::pair("  +\n-\t("s, std::vector({
        cppinky::Token(cppinky::TokenType::PLUS, "+")
      , cppinky::Token(cppinky::TokenType::MINUS, "-")
      , cppinky::Token(cppinky::TokenType::LEFT_PAREN, "(")
      }))
    );
    auto tokenizer = cppinky::Tokenizer(script);

    auto result = tokenizer.tokenize();
    REQUIRE(result.size() == tokens.size());

    for (auto [result, token]: std::views::zip(result, tokens))
    {
      REQUIRE(result.type == token.type);
      REQUIRE(result.lexeme == token.lexeme);
    }
  }
}
