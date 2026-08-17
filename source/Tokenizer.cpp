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

    switch (character)
    {
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
