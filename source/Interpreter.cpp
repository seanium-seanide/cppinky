#include <Interpreter.hpp>

#include <Tokenizer.hpp>
#include <argparse/argparse.hpp>
#include <cstdlib>
#include <filesystem>
#include <utilities.hpp>

namespace pinky
{

Interpreter::Interpreter(const int argc, char**argv)
{
  parseCommandLineArgs(argc, argv);

  m_script = utilities::readTextFile(m_scriptFileName);
}

auto Interpreter::parseCommandLineArgs(int argc, char** argv) -> void
{
  argparse::ArgumentParser program("cppinky");

  program.add_argument("scriptFileName")
         .help("File mame for pinky script to target");

  try
  {
    program.parse_args(argc, argv);
  }
  catch (const std::exception& e)
  {
    fmt::println(stderr, "{}", e.what());
    std::cerr << program;
    std::exit(1);
  }

  m_scriptFileName = program.get("scriptFileName");

  if (!std::filesystem::is_regular_file(m_scriptFileName))
  {
    fmt::println("Error: {} is not a regular file", m_scriptFileName);
    std::exit(1);
  }
}

auto Interpreter::run() const -> int
{
  auto tokenizer = Tokenizer(m_script);
  const auto tokens = tokenizer.tokenize();

  for (auto& token: tokens)
  {
    fmt::println("{}", token);
  }

  return 0;
}

}
