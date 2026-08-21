#include <utilities.hpp>

#include <fstream>
#include <fmt/format.h>
#include <filesystem>


namespace utilities
{

auto readStream(std::istream& is) -> std::string
{
  return {std::istreambuf_iterator<char>{is}, {}};
}

auto readTextFile(const std::string& filename) -> std::string
{
  auto file = std::ifstream(filename);

  if (!file)
  {
    throw std::runtime_error(
      fmt::format(
        "Failed to open file {}. Current directory: {}"
      , filename, std::string{std::filesystem::current_path()}
      )
    );
  }

  auto script = readStream(file);

  file.close();

  return script;
}

auto isdigit(pinky::char_type c) -> bool
{
  auto character = static_cast<unsigned char>(c);

  if (character < '0' || character > '9')
  {
    return false;
  }

  return true;
}

auto isalpha(pinky::char_type c) -> bool
{
  auto character = static_cast<unsigned char>(c);

  if (!(character >= 'A' && character <= 'Z') && !(character >= 'a' && character <= 'z') && character != '_')
  {
    return false;
  }

  return true;
}

auto isspace(pinky::char_type c) -> bool
{
  auto character = static_cast<unsigned char>(c);

  switch (character)
  {
    case ' ':
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case '\v':
    {
      return true;
      break;
    }

    default:
    {
      return false;
      break;
    }
  }
}

}
