#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <Tokenizer.hpp>
#include <ranges>
#include <stdexcept>
#include <vector>

using namespace std::literals;
using pinky::Token;
using pinky::TokenType;
using pinky::Tokenizer;
using PairType = std::pair<std::string, std::vector<Token>>;


TEST_CASE("Single character tokens", "[cppinky][tokenizer][single]")
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
      std::tuple("(", TokenType::LEFT_PAREN, "(")
    , std::tuple(")", TokenType::RIGHT_PAREN, ")")
    , std::tuple("{", TokenType::LEFT_CURLY, "{")
    , std::tuple("}", TokenType::RIGHT_CURLY, "}")
    , std::tuple("[", TokenType::LEFT_SQUARE, "[")
    , std::tuple("]", TokenType::RIGHT_SQUARE, "]")
    , std::tuple(".", TokenType::DOT, ".")
    , std::tuple(",", TokenType::COMMA, ",")
    , std::tuple("+", TokenType::PLUS, "+")
    , std::tuple("-", TokenType::MINUS, "-")
    , std::tuple("*", TokenType::TIMES, "*")
    , std::tuple("/", TokenType::DIVIDE, "/")
    , std::tuple("<", TokenType::LESS, "<")
    , std::tuple(">", TokenType::GREATER, ">")
    , std::tuple("~", TokenType::NOT, "~")
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

  SECTION("When a whitespace is encountered, a token is not emitted.")
  {
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

  SECTION("When a comment is encountered, the remainder of the line is ignored.")
  {
    auto [script, expectedTokens] = GENERATE(
      PairType{"#", {}}
    , PairType{"#\n", {}}
    , PairType{"  #  ", {}}
    , PairType{"  #  \n", {}}
    , PairType{"- # +", {{TokenType::MINUS, "-"}}}
    , PairType{"- # +\n", {{TokenType::MINUS, "-"}}}
    , PairType{"+ # */- This is a comment\n(", {{TokenType::PLUS, "+"}, {TokenType::LEFT_PAREN, "("}}}
    , PairType{"+ # */- This is a comment\n(  # Comment \n", {{TokenType::PLUS, "+"}, {TokenType::LEFT_PAREN, "("}}}
    , PairType{"+ # */- This is a comment\n( *  # Comment / \n", {{TokenType::PLUS, "+"}, {TokenType::LEFT_PAREN, "("}, {TokenType::TIMES, "*"}}}
    );

    auto tokenizer = Tokenizer{script};
    auto result = tokenizer.tokenize();
    REQUIRE(result.size() == expectedTokens.size());
    for (auto [resultToken, expectedToken]: std::views::zip(result, expectedTokens))
    {
      REQUIRE(resultToken.type == expectedToken.type);
      REQUIRE(resultToken.lexeme == expectedToken.lexeme);
    }
  }

  SECTION("Tokens are stored with the correct line number .")
  {
    using ThisPairType = std::pair<std::string, std::vector<pinky::size_type>>;

    auto [script, expectedLineNumbers] = GENERATE(
      ThisPairType{")\n", {1}}
    , ThisPairType{"+\n-\n*\n/\n", {1, 2, 3, 4}}
    , ThisPairType{"+\n-\n\n*\n/\n", {1, 2, 4, 5}}
    , ThisPairType{"+\n-\n\n*\n/\n\n\n+++\n-", {1, 2, 4, 5, 8, 8, 8, 9}}
    );

    auto tokenizer = Tokenizer{script};
    auto result = tokenizer.tokenize();

    for (auto [token, lineNumber]: std::views::zip(result, expectedLineNumbers))
    {
      REQUIRE(token.lineNumber == lineNumber);
    }
  }
}

TEST_CASE("Two character tokens", "[cppinky][tokenizer][multi]")
{
  SECTION("When a valid two-character token is encountered, a token is emitted")
  {
    using ThisPairType = std::pair<std::string, Token>;
    auto [script, token] = GENERATE(
      ThisPairType{"==", {TokenType::EQUAL, "=="}}
    , ThisPairType{"<=", {TokenType::LESS_EQUAL, "<="}}
    , ThisPairType{">=", {TokenType::GREATER_EQUAL, ">="}}
    , ThisPairType{"~=", {TokenType::NOT_EQUAL, "~="}}
    , ThisPairType{":=", {TokenType::ASSIGN, ":="}}
    );

    auto tokenizer = Tokenizer(script);
    auto tokens = tokenizer.tokenize();

    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens.back().type == token.type);
    REQUIRE(tokens.back().lexeme == token.lexeme);
  }

  SECTION("When many tokens two-character tokens are encountered, many tokens are emitted")
  {
    auto [script, expectedTokens] = GENERATE(
     PairType{"== ~= <= >= :=\n"
     , { {TokenType::EQUAL, "=="},         {TokenType::NOT_EQUAL, "~="}, {TokenType::LESS_EQUAL, "<="}
       , {TokenType::GREATER_EQUAL, ">="}, {TokenType::ASSIGN, ":="}}
     }
     , PairType{"== ~= \n" 
                "<= >= # ~= !=\n"
                " :=\n"
     , { {TokenType::EQUAL, "=="},         {TokenType::NOT_EQUAL, "~="}, {TokenType::LESS_EQUAL, "<="}
       , {TokenType::GREATER_EQUAL, ">="}, {TokenType::ASSIGN, ":="}}
     }
    );

    auto tokenizer = Tokenizer(script);
    auto results = tokenizer.tokenize();

    REQUIRE(results.size() == expectedTokens.size());
    for (auto [token, result]: std::views::zip(expectedTokens, results))
    {
      REQUIRE(token.type == result.type);
      REQUIRE(token.lexeme == result.lexeme);
    }
  }

  SECTION("When a partial match for a two-character token is encountered, an exception is thrown")
  {
    auto script = "=";

    auto tokenizer = Tokenizer(script);

    REQUIRE_THROWS_AS(tokenizer.tokenize(), std::runtime_error);
  }

  SECTION("When a partial match for a two-character token is encountered, an exception for an invalid token is thrown")
  {
    auto script = "=";

    auto tokenizer = Tokenizer(script);

    REQUIRE_THROWS_WITH(tokenizer.tokenize(), Catch::Matchers::ContainsSubstring("Invalid token"));
  }

  SECTION("When an integer lexeme is encountered, the corresponding token is emitted")
  {
    // digit := '0' | '1' | ... | '9'
    // integer := digit*
    auto script = "123";

    auto tokenizer = Tokenizer(script);
    auto tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 1);

    REQUIRE(tokens.back().type == TokenType::NUMBER);
    REQUIRE(tokens.back().lexeme == "123");
  }

  SECTION("When floating-point lexeme is encountered, the corresponding token is emitted")
  {
    using ThisPairType = std::pair<std::string, Token>;

    // float := integer? '.' integer
    auto [script, output] = GENERATE(
      ThisPairType{"123.456", {TokenType::NUMBER, "123.456"}}
    , ThisPairType{"123.456\n", {TokenType::NUMBER, "123.456"}}
    );

    auto tokenizer = Tokenizer(script);
    auto tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 1);

    REQUIRE(tokens.back().type == output.type);
    REQUIRE(tokens.back().lexeme == output.lexeme);
  }

  SECTION("When multiple floating-point lexemes are encountered on a single line, all of the corresponding tokens are emitted")
  {
    auto [script, output] = GENERATE(
      PairType{" 123.456     789.123", {{TokenType::NUMBER, "123.456"}, {TokenType::NUMBER, "789.123"}}}
    , PairType{" 123.456     789.123\n", {{TokenType::NUMBER, "123.456"}, {TokenType::NUMBER, "789.123"}}}
    );

    auto tokenizer = Tokenizer(script);
    auto tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == output.size());

    for (auto [token, expectedToken]: std::views::zip(output, tokens))
    {
      REQUIRE(token.type == expectedToken.type);
      REQUIRE(token.lexeme == expectedToken.lexeme);
    }
  }

  SECTION("When floating-point lexemes are encountered on multiple lines, all of the corresponding tokens are emitted")
  {
    auto [script, output] = GENERATE(
      PairType{" 123.456  \n   789.123", {{TokenType::NUMBER, "123.456"}, {TokenType::NUMBER, "789.123"}}}
    , PairType{" 123.456  \n 1.2  789.123 3.4    \n 5.6"
      , {
          {TokenType::NUMBER, "123.456"}, {TokenType::NUMBER, "1.2"}, {TokenType::NUMBER, "789.123"}
        , {TokenType::NUMBER, "3.4"}, {TokenType::NUMBER, "5.6"}
        }
      }
    );

    auto tokenizer = Tokenizer(script);
    auto tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == output.size());

    for (auto [token, expectedToken]: std::views::zip(output, tokens))
    {
      REQUIRE(token.type == expectedToken.type);
      REQUIRE(token.lexeme == expectedToken.lexeme);
    }
  }

  SECTION("When the tokenizer encounters a string, a string token is emitted")
  {
    auto script = "\"This is a string\"";

    auto tokenizer = Tokenizer(script);
    auto tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == 1);

    REQUIRE(tokens.back().type == TokenType::STRING);
    REQUIRE(tokens.back().lexeme == "This is a string");
  }
}
