#include <Interpreter.hpp>

#include <filesystem>
#include <cstdlib>
#include <fmt/core.h>
#include <argparse/argparse.hpp>
#include <utilities.hpp>
#include <Tokenizer.hpp>


namespace cppinky
{

Interpreter::Interpreter(int argc, char**argv)
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

  if (!std::filesystem::exists(m_scriptFileName))
  {
    fmt::println("Error: File {} doesn't exist", m_scriptFileName);
    std::exit(1);
  }
}

auto Interpreter::run() -> int
{
  auto tokenizer = Tokenizer(m_script);
  auto tokens = tokenizer.tokenize();

  for (auto& token: tokens)
  {
    fmt::println("{}", token);
  }

  return 0;
}

}
