#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <Token.hpp>


namespace cppinky
{

class Tokenizer
{
public:
  using char_type = char;
  using size_type = std::size_t;

  Tokenizer(std::string_view source);

  auto tokenize() -> std::vector<Token>;

private:
  std::vector<Token> m_tokens{};
  std::string m_source{};
  size_type m_currentIndex{};
  size_type m_startIndex{};
  size_type m_lineIndex{1};

  auto skipWhitespace() -> void;
  auto addToken(TokenType type) -> void;
  auto advance() -> Tokenizer::char_type;
  auto current() -> char_type;
  auto peek() -> char_type;
  auto match(char_type expectedChar) -> bool;
};

}
