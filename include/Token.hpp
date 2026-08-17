#pragma once

#include <ostream>
#include <fmt/format.h>
#include <magic_enum/magic_enum.hpp>


namespace cppinky
{

enum class TokenType
{
  // Single-character tokens
  LEFT_PAREN
, RIGHT_PAREN
, LEFT_CURLY
, RIGHT_CURLY
, LEFT_SQUARE
, RIGHT_SQUARE
, DOT
, COMMA
, PLUS
, MINUS
, TIMES
, DIVIDE
  // Two-character tokens
  // Literals
  // Reserved words
};


struct Token
{
  TokenType type;
  std::string_view lexeme;
  // TODO: This should be Tokenizer::size_type
  std::size_t lineNumber;
};

auto operator<<(std::ostream& os, const cppinky::Token& token) -> std::ostream&;

}


#include <Token.tpp>
