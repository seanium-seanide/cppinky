#include <Tokenizer.hpp>


namespace pinky
{

Tokenizer::Tokenizer(std::string_view source)
: m_source(source)
{
}

auto Tokenizer::skipWhitespace() -> void
{
  while (m_currentIndex < m_source.size() && std::isspace(current()))
  {
    if (current() == '\n')
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

  while ((character = advance()) != '\n')
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
  }
  else
  {
    if (opt2 != std::nullopt)
    {
      addToken(opt2.value());
    }
  }
}

auto Tokenizer::tokenize() -> std::span<Token>
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
        if (match('='))
        {
          addToken(TokenType::EQUAL);
        }
        else
        {
          invalidToken();
        }

        break;
      }

      case '<':
      {
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
      }

      case '>':
      {
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
      }

      case '~':
      {
        addToken(match('=') ? TokenType::NOT_EQUAL : TokenType::NOT);
        break;
      }

      case ':':
      {
        if (match('='))
        {
          addToken(TokenType::ASSIGN);
        }
        else
        {
          invalidToken();
        }

        break;
      }

      default:
      {
        invalidToken();

        break;
      }
    }
  }

  return m_tokens;
}

auto Tokenizer::advance() -> char_type
{
  return m_source[m_currentIndex++];
}

auto Tokenizer::current() -> char_type
{
  return m_source[m_currentIndex];
}

// TODO: Plumb in std::optional
auto Tokenizer::peek() -> std::optional<char_type>
{
  if (m_currentIndex >= m_source.size())
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
