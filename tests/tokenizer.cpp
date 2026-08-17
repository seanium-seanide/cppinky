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
    auto script = ""s;
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
    auto [script, tokens] = GENERATE(
      std::pair("()[]{}.,+-*/\n"s, std::vector({
        Token(TokenType::LEFT_PAREN, "(")
      , Token(TokenType::RIGHT_PAREN, ")")
      , Token(TokenType::LEFT_SQUARE, "[")
      , Token(TokenType::RIGHT_SQUARE, "]")
      , Token(TokenType::LEFT_CURLY, "{")
      , Token(TokenType::RIGHT_CURLY, "}")
      , Token(TokenType::DOT, ".")
      , Token(TokenType::COMMA, ",")
      , Token(TokenType::PLUS, "+")
      , Token(TokenType::MINUS, "-")
      , Token(TokenType::TIMES, "*")
      , Token(TokenType::DIVIDE, "/")
      }))
    , std::pair("**-+-/-+**/\n", std::vector({
        Token(TokenType::TIMES, "*")
      , Token(TokenType::TIMES, "*")
      , Token(TokenType::MINUS, "-")
      , Token(TokenType::PLUS, "+")
      , Token(TokenType::MINUS, "-")
      , Token(TokenType::DIVIDE, "/")
      , Token(TokenType::MINUS, "-")
      , Token(TokenType::PLUS, "+")
      , Token(TokenType::TIMES, "*")
      , Token(TokenType::TIMES, "*")
      , Token(TokenType::DIVIDE, "/")
      }))
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
    auto [script, tokens] = GENERATE(
      std::pair("        +\n"s, std::vector({
        Token(TokenType::PLUS, "+")
      }))
    , std::pair("\n\n\n\n\n+\n"s, std::vector({
        Token(TokenType::PLUS, "+")
      }))
    , std::pair("  +\n-\t(\n"s, std::vector({
        Token(TokenType::PLUS, "+")
      , Token(TokenType::MINUS, "-")
      , Token(TokenType::LEFT_PAREN, "(")
      }))
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
    auto [script, expectedTokens] = GENERATE(
      std::pair<std::string, std::vector<Token>>(
        "+ # */- This is a comment\n("s
      , {Token(TokenType::PLUS, "+"), Token(TokenType::LEFT_PAREN, "(")}
      )
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
