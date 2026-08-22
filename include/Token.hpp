#pragma once

#include <ostream>
#include <fmt/format.h>
#include <magic_enum/magic_enum.hpp>
#include <TokenizerTypes.hpp>
#include <unordered_map>

using namespace std::literals;


namespace pinky
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
, LESS
, GREATER
, NOT
  // Two-character tokens
, EQUAL
, LESS_EQUAL
, GREATER_EQUAL
, NOT_EQUAL
, ASSIGN
  // Literals
, NUMBER
, STRING
, IDENTIFIER
  // Reserved words
, KW_IF
, KW_THEN
, KW_ELSE
, KW_TRUE
, KW_FALSE
, KW_AND
, KW_OR
, KW_WHILE
, KW_DO
, KW_FOR
, KW_FUNC
, KW_END
, KW_LOCAL
, KW_PRINT
, KW_PRINTLN
, KW_RET
};

struct Token
{
  TokenType type;
  std::string_view lexeme;
  size_type lineNumber;
};


auto operator<<(std::ostream& os, const pinky::Token& token) -> std::ostream&;

}


#include <Token.tpp>
