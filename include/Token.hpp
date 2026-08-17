#pragma once

#include <ostream>
#include <fmt/format.h>
#include <magic_enum/magic_enum.hpp>


namespace cppinky
{

enum class TokenType
{
  // Single-character tokens
  PLUS
, MINUS
, TIMES
, DIVIDE
  // Two-character rokens
  // Literals
  // Reserved words
};


// TODO: Move out to separate file? (Probably same file as TokenType if at all)
struct Token
{
  TokenType type;
  std::string lexeme;
};

auto operator<<(std::ostream& os, const cppinky::Token& token) -> std::ostream&;
}


#include <Token.tpp>
