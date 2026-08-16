#include <utilities.hpp>

#include <fstream>
#include <format>
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
      std::format(
        "Failed to open file {}. Current directory: {}"
      , filename, std::string{std::filesystem::current_path()}
      )
    );
  }

  auto script = readStream(file);

  file.close();

  return script;
}

}
