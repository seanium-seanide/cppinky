#include <Tokenizer.hpp>


namespace cppinky
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

      default:
      {
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

auto Tokenizer::peek() -> char_type 
{
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

}
