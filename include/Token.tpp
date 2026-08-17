template<>
struct fmt::formatter<cppinky::Token>
{
  constexpr auto parse(fmt::format_parse_context& context)
  {
    return context.begin();
  }

  auto format(const cppinky::Token& token, fmt::format_context& context) const
  {
    return fmt::format_to(
      context.out()
    , "[{}, {}, {}]"
    , magic_enum::enum_name(token.type)
    , token.lexeme
    , token.lineNumber
    );
  }
};
