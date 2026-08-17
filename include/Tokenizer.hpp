#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <span>
#include <Token.hpp>


namespace cppinky
{

class Tokenizer
{
public:
  using char_type = char;
  using size_type = std::size_t;

  Tokenizer(std::string_view source);

  auto tokenize() -> std::span<Token>;

private:
  std::vector<Token> m_tokens{};
  std::string m_source{};
  size_type m_currentIndex{};
  size_type m_startIndex{};
  size_type m_currentLineIndex{1};

  auto skipWhitespace() -> void;
  auto stepOverComment() -> void;
  auto addToken(TokenType type) -> void;
  auto advance() -> Tokenizer::char_type;
  auto current() -> char_type;
  auto peek() -> char_type;
  auto match(char_type expectedChar) -> bool;
};

}
