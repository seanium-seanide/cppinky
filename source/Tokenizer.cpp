#include <Tokenizer.hpp>


namespace pinky
{

Tokenizer::Tokenizer(std::string_view source)
: m_source(source)
{
}

auto Tokenizer::skipWhitespace() -> void
{
  while (m_currentIndex < m_source.size() && std::isspace(static_cast<unsigned char>(current().value())))
  {
    if (current().value() == '\n')
    {
      ++m_currentLineIndex;
    }

    static_cast<void>(advance());
  }
}

auto Tokenizer::addToken(TokenType type) -> void
{
  m_tokens.emplace_back(type, std::string_view{m_source}.substr(m_startIndex, m_currentIndex - m_startIndex), m_currentLineIndex);
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
      case '#':
      {
        stepOverComment();
        break;
      }

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
        addToken(TokenType::MINUS);
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
      {
        scanString();
        break;
      }

      default:
      {
        if (std::isdigit(static_cast<unsigned char>(character)))
        {
          scanDigit();
        }
        else
        {
          invalidToken();
        }

        break;
      }

      // TODO: Strings
      // TODO: Identifiers
    }
  }

  return m_tokens;
}

auto Tokenizer::scanDigit() -> void
{
  while (current() != std::nullopt && std::isdigit(static_cast<unsigned char>(current().value())))
  {
    static_cast<void>(advance());
  }

  if (current() != std::nullopt && current().value() == '.' && peek() != std::nullopt && std::isdigit(static_cast<unsigned char>(peek().value())))
  {
    static_cast<void>(advance());

    while (current() != std::nullopt && std::isdigit(static_cast<unsigned char>(current().value())))
    {
      static_cast<void>(advance());
    }
  }

  addToken(TokenType::NUMBER);
}

auto Tokenizer::scanString() -> void
{
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
