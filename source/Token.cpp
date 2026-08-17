#include <Token.hpp>


namespace pinky
{

auto operator<<(std::ostream& os, const Token& token) -> std::ostream&
{
  fmt::format_to(std::ostreambuf_iterator<char>{os}, "{}", token);

  return os;
}

}
