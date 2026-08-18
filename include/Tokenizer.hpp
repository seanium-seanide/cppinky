#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <span>
#include <Token.hpp>
#include <TokenizerTypes.hpp>


namespace pinky
{

class Tokenizer
{
public:
  Tokenizer(std::string_view source);

  [[nodiscard]] auto tokenize() -> std::span<Token>;

private:
  std::vector<Token> m_tokens{};
  std::string m_source{};
  size_type m_currentIndex{};
  size_type m_startIndex{};
  size_type m_currentLineIndex{1};

  auto skipWhitespace() -> void;
  auto stepOverComment() -> void;
  auto addToken(TokenType type) -> void;
  auto advance() -> char_type;
  auto current() -> char_type;
  auto peek() -> char_type;
  auto match(char_type expectedChar) -> bool;
};

}
