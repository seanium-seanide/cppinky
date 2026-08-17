#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <vector>
#include <ranges>
#include <utilities.hpp>
#include <Tokenizer.hpp>

using namespace std::literals;
using cppinky::Token;
using cppinky::TokenType;
using cppinky::Tokenizer;


TEST_CASE("Tokenizing scripts", "[cppinky]")
{
  SECTION("When tokenizing an empty script, an empty sequence of tokens is returned.")
  {
    auto script = "";
    auto tokenizer = Tokenizer{script};

    auto result = tokenizer.tokenize();

    REQUIRE(result.size() == 0);
  }

  SECTION("When tokenizing a script containing a single character, a single token is returned.")
  {
    auto [script, expectedTokenType, expectedLexeme] = GENERATE(
      std::tuple("+", TokenType::PLUS, "+")
    , std::tuple("-", TokenType::MINUS, "-")
    , std::tuple("*", TokenType::TIMES, "*")
    , std::tuple("/", TokenType::DIVIDE, "/")
    );
    auto tokenizer = Tokenizer{script};

    auto result = tokenizer.tokenize();

    REQUIRE(result.size() == 1);
    REQUIRE(result.back().type == expectedTokenType);
    REQUIRE(result.back().lexeme == expectedLexeme);
  }

  SECTION("When tokenizing a script, the correct sequence of lexemes is returned")
  {
    using PairType = std::pair<std::string, std::vector<Token>>;

    auto [script, tokens] = GENERATE(
      PairType("()[]{}.,+-*/\n", {
        {TokenType::LEFT_PAREN, "("}
      , {TokenType::RIGHT_PAREN, ")"}
      , {TokenType::LEFT_SQUARE, "["}
      , {TokenType::RIGHT_SQUARE, "]"}
      , {TokenType::LEFT_CURLY, "{"}
      , {TokenType::RIGHT_CURLY, "}"}
      , {TokenType::DOT, "."}
      , {TokenType::COMMA, ","}
      , {TokenType::PLUS, "+"}
      , {TokenType::MINUS, "-"}
      , {TokenType::TIMES, "*"}
      , {TokenType::DIVIDE, "/"}
      })
    , PairType("**-+-/-+**/\n", {
        {TokenType::TIMES, "*"}
      , {TokenType::TIMES, "*"}
      , {TokenType::MINUS, "-"}
      , {TokenType::PLUS, "+"}
      , {TokenType::MINUS, "-"}
      , {TokenType::DIVIDE, "/"}
      , {TokenType::MINUS, "-"}
      , {TokenType::PLUS, "+"}
      , {TokenType::TIMES, "*"}
      , {TokenType::TIMES, "*"}
      , {TokenType::DIVIDE, "/"}
      })
    );
    auto tokenizer = Tokenizer{script};

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
    using PairType = std::pair<std::string, std::vector<Token>>;

    auto [script, tokens] = GENERATE(
      PairType("        +\n", {{TokenType::PLUS, "+"}})
    , PairType("\n\n\n\n\n+\n", {{TokenType::PLUS, "+"}})
    , PairType("  +\n-\t(\n", {{TokenType::PLUS, "+"}, {TokenType::MINUS, "-"}, {TokenType::LEFT_PAREN, "("}})
    );
    auto tokenizer = Tokenizer{script};

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
    using PairType = std::pair<std::string, std::vector<Token>>;

    auto [script, expectedTokens] = GENERATE(
      PairType{"+ # */- This is a comment\n(", {{TokenType::PLUS, "+"}, {TokenType::LEFT_PAREN, "("}}}
    );

    auto tokenizer = Tokenizer{script};
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
