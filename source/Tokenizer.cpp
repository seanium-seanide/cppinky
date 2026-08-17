#include <Tokenizer.hpp>


namespace cppinky
{

Tokenizer::Tokenizer(std::string_view source)
: m_source(source)
{
}

// TODO: Move to string views?
auto Tokenizer::addToken(TokenType type) -> void
{
  m_tokens.emplace_back(type, m_source.substr(m_startIndex, m_currentIndex - m_startIndex));
}

// TODO: Move to string views?
// TODO: Return a read-only view?
auto Tokenizer::tokenize() -> std::vector<Token>
{
  while (m_currentIndex < m_source.size())
  {
    m_startIndex = m_currentIndex;
    auto character = advance();

    /*
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
  */
    switch (character)
    {
      case '\n':
      {
        ++m_lineIndex;
        break;
      }

      case ' ':
      case '\r':
      case '\t':
      {
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

auto Tokenizer::advance() -> Tokenizer::char_type
{
  return m_source[m_currentIndex++];
}

auto Tokenizer::peek() -> Tokenizer::char_type 
{
  return m_source[m_currentIndex + 1];
}

auto Tokenizer::match(Tokenizer::char_type expectedChar) -> bool
{
  if (m_source[m_currentIndex] != expectedChar)
  {
    return false;
  }

  ++m_currentIndex;
  return true;
}

}
