#include <Interpreter.hpp>

#include <filesystem>
#include <cstdlib>
#include <fmt/core.h>
#include <argparse/argparse.hpp>


namespace cppinky
{

Interpreter::Interpreter(int argc, char**argv)
{
  parseCommandLineArgs(argc, argv);

  fmt::println("Pinky script: {}", m_scriptFileName);
}

void Interpreter::parseCommandLineArgs(int argc, char** argv)
{
  static_cast<void>(argc);
  static_cast<void>(argv);

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

int Interpreter::run()
{
  return {};
}

}
