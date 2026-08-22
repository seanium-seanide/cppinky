#include <Tokenizer.hpp>

#include <algorithm>
#include <utilities.hpp>


namespace pinky
{

Tokenizer::Tokenizer(std::string_view source)
: m_source(source)
{
}

auto Tokenizer::skipWhitespace() -> void
{
  while (m_currentIndex < m_source.size() && std::isspace(current().value()))
  {
    if (current().value() == '\n')
    {
      ++m_currentLineIndex;
    }

    static_cast<void>(advance());
  }
}

auto Tokenizer::getLexeme() -> std::string_view const
{
  return std::string_view{m_source}.substr(m_startIndex, m_currentIndex - m_startIndex);
}

auto Tokenizer::addToken(TokenType type) -> void
{
  m_tokens.emplace_back(type, getLexeme(), m_currentLineIndex);
}

auto Tokenizer::stepOverComment() -> void
{
  auto character = char_type{};

  while (m_currentIndex < m_source.size() && (character = advance()) != '\n')
  {
  }

  if (character == '\n')
  {
    ++m_currentLineIndex;
  }
}

auto Tokenizer::addTokenWithPredicate(bool predicate, std::optional<TokenType> opt1, std::optional<TokenType> opt2) -> void
{
  if (predicate)
  {
    if (opt1 != std::nullopt)
    {
      addToken(opt1.value());
    }
    else
    {
      invalidToken();
    }
  }
  else
  {
    if (opt2 != std::nullopt)
    {
      addToken(opt2.value());
    }
    else
    {
      invalidToken();
    }
  }
}

auto Tokenizer::tokenize() -> std::span<const Token>
{
  while (m_currentIndex < m_source.size())
  {
    skipWhitespace();

    if (m_currentIndex >= m_source.size())
    {
      break;
    }

    m_startIndex = m_currentIndex;
    auto character = advance();

    switch (character)
    {
      case '(':
      {
        addToken(TokenType::LEFT_PAREN);
        break;
      }

      case ')':
      {
        addToken(TokenType::RIGHT_PAREN);
        break;
      }

      case '{':
      {
        addToken(TokenType::LEFT_CURLY);
        break;
      }

      case '}':
      {
        addToken(TokenType::RIGHT_CURLY);
        break;
      }

      case '[':
      {
        addToken(TokenType::LEFT_SQUARE);
        break;
      }

      case ']':
      {
        addToken(TokenType::RIGHT_SQUARE);
        break;
      }

      case '.':
      {
        addToken(TokenType::DOT);
        break;
      }

      case ',':
      {
        addToken(TokenType::COMMA);
        break;
      }

      case '+':
      {
        addToken(TokenType::PLUS);
        break;
      }

      case '-':
      {
        if (match('-'))
        {
          stepOverComment();
        }
        else
        {
          addToken(TokenType::MINUS);
        }

        break;
      }

      case '*':
      {
        addToken(TokenType::TIMES);
        break;
      }

      case '/':
      {
        addToken(TokenType::DIVIDE);
        break;
      }

      case '=':
      {
        addTokenWithPredicate(match('='), TokenType::EQUAL, std::nullopt);

        break;
      }

      case '<':
      {
        addTokenWithPredicate(match('='), TokenType::LESS_EQUAL, TokenType::LESS);
        break;
      }

      case '>':
      {
        addTokenWithPredicate(match('='), TokenType::GREATER_EQUAL, TokenType::GREATER);
        break;
      }

      case '~':
      {
        addTokenWithPredicate(match('='), TokenType::NOT_EQUAL, TokenType::NOT);
        break;
      }

      case ':':
      {
        addTokenWithPredicate(match('='), TokenType::ASSIGN, std::nullopt);
        break;
      }

      case '\"':
      case '\'':
      {
        scanString(character);
        break;
      }

      default:
      {
        if (utilities::isdigit(character))
        {
          scanDigit();
        }
        else if(utilities::isalpha(character))
        {
          scanIdentifier();
        }
        else
        {
          invalidToken();
        }

        break;
      }
    }
  }

  return m_tokens;
}

auto Tokenizer::scanIdentifier() -> void
{
  while (current() != std::nullopt && utilities::isalnum(current().value()))
  {
    static_cast<void>(advance());
  }

  auto name = "KW_"s;
  std::ranges::transform(
    getLexeme()
  , std::back_inserter(name)
  , [](unsigned char c)
    {
      return static_cast<char>(std::toupper(c));
    }
  );

  if (auto token = magic_enum::enum_cast<TokenType>(name))
  {
    addToken(*token);
  }
  else
  {
    addToken(TokenType::IDENTIFIER);
  }
}

auto Tokenizer::scanString(char_type delimiter) -> void
{
  if (current() != std::nullopt && current().value() != delimiter)
  {
    static_cast<void>(advance());
    ++m_startIndex;
  }

  while (current() != std::nullopt && current().value() != delimiter)
  {
    static_cast<void>(advance());
  }

  addToken(TokenType::STRING);
  static_cast<void>(advance());
}

auto Tokenizer::scanDigit() -> void
{
  while (current() != std::nullopt && std::isdigit(current().value()))
  {
    static_cast<void>(advance());
  }

  if (current() != std::nullopt && current().value() == '.' && peek() != std::nullopt && std::isdigit(peek().value()))
  {
    static_cast<void>(advance());

    while (current() != std::nullopt && std::isdigit(current().value()))
    {
      static_cast<void>(advance());
    }
  }

  addToken(TokenType::NUMBER);
}

auto Tokenizer::advance() -> char_type
{
  return m_source[m_currentIndex++];
}

auto Tokenizer::current() -> std::optional<char_type>
{
  if (m_currentIndex >= m_source.size())
  {
    return std::nullopt;
  }

  return m_source[m_currentIndex];
}

auto Tokenizer::peek() -> std::optional<char_type>
{
  if (m_currentIndex + 1 >= m_source.size())
  {
    return std::nullopt;
  }

  return m_source[m_currentIndex + 1];
}

auto Tokenizer::match(char_type expectedChar) -> bool
{
  if (m_currentIndex >= m_source.size())
  {
    return false;
  }

  if (m_source[m_currentIndex] != expectedChar)
  {
    return false;
  }

  ++m_currentIndex;
  return true;
}

auto Tokenizer::invalidToken() -> void
{
  throw std::runtime_error("Invalid token");
}

}
