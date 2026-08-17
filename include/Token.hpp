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


// TODO: Move out to separate file? (Probably same file as TokenType if at all)
struct Token
{
  TokenType type;
  std::string lexeme;
  std::size_t currentLine;
};

auto operator<<(std::ostream& os, const cppinky::Token& token) -> std::ostream&;

}


#include <Token.tpp>
