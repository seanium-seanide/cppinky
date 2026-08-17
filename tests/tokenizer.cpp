#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <vector>
#include <ranges>
#include <utilities.hpp>
#include <Tokenizer.hpp>

using namespace std::literals;


TEST_CASE("Tokenizing scripts", "[cppinky]")
{
  SECTION("When tokenizing an empty script, an empty sequence of tokens is returned.")
  {
    auto script = ""s;
    auto tokenizer = cppinky::Tokenizer{script};

    auto result = tokenizer.tokenize();

    REQUIRE(result.size() == 0);
  }

  SECTION("When tokenizing a script containing a single character, a single token is returned.")
  {
    auto [script, expectedTokenType, expectedLexeme] = GENERATE(
      std::tuple("+", cppinky::TokenType::PLUS, "+")
    , std::tuple("-", cppinky::TokenType::MINUS, "-")
    , std::tuple("*", cppinky::TokenType::TIMES, "*")
    , std::tuple("/", cppinky::TokenType::DIVIDE, "/")
    );
    auto tokenizer = cppinky::Tokenizer{script};

    auto result = tokenizer.tokenize();

    REQUIRE(result.size() == 1);
    REQUIRE(result.back().type == expectedTokenType);
    REQUIRE(result.back().lexeme == expectedLexeme);
  }

  SECTION("When tokenizing a script, the correct sequence of lexemes is returned")
  {
    auto [script, tokens] = GENERATE(
      std::pair("()[]{}.,+-*/\n"s, std::vector({
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
    , std::pair("**-+-/-+**/\n", std::vector({
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
    auto tokenizer = cppinky::Tokenizer{script};

    auto result = tokenizer.tokenize();
    REQUIRE(result.size() == tokens.size());

    for (auto [result, token]: std::views::zip(result, tokens))
    {
      REQUIRE(result.type == token.type);
      REQUIRE(result.lexeme == token.lexeme);
    }
  }

  SECTION("When the tokenizer encounters whitespace, a token is not emitted.")
  {
    auto [script, tokens] = GENERATE(
      std::pair("        +\n"s, std::vector({
        cppinky::Token(cppinky::TokenType::PLUS, "+")
      }))
    , std::pair("\n\n\n\n\n+\n"s, std::vector({
        cppinky::Token(cppinky::TokenType::PLUS, "+")
      }))
    , std::pair("  +\n-\t(\n"s, std::vector({
        cppinky::Token(cppinky::TokenType::PLUS, "+")
      , cppinky::Token(cppinky::TokenType::MINUS, "-")
      , cppinky::Token(cppinky::TokenType::LEFT_PAREN, "(")
      }))
    );
    auto tokenizer = cppinky::Tokenizer{script};

    auto result = tokenizer.tokenize();
    REQUIRE(result.size() == tokens.size());

    for (auto [result, token]: std::views::zip(result, tokens))
    {
      REQUIRE(result.type == token.type);
      REQUIRE(result.lexeme == token.lexeme);
    }
  }

  SECTION("When the tokenizer encounters a comment, the remainder of the line is ignored.")
  {
    auto script = "+ # */- This is a comment\n("s;

    auto expectedTokens = std::vector({
      cppinky::Token(cppinky::TokenType::PLUS, "+")
    , cppinky::Token(cppinky::TokenType::LEFT_PAREN, "(")
    });
    auto tokenizer = cppinky::Tokenizer{script};

    auto result = tokenizer.tokenize();
    REQUIRE(result.size() == expectedTokens.size());
    for (auto [resultToken, expectedToken]: std::views::zip(result, expectedTokens))
    {
      // TODO: Implement token comparison operator and refactor tests
      REQUIRE(resultToken.type == expectedToken.type);
      REQUIRE(resultToken.lexeme == expectedToken.lexeme);
    }
  }
}
