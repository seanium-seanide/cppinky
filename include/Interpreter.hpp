#pragma once

#include <string>


namespace pinky
{

class Interpreter
{
public:
  Interpreter() = default;
  Interpreter(int argc, char**argv);

  auto run() -> int;

private:
  std::string m_scriptFileName;
  std::string m_script;

  auto parseCommandLineArgs(int argc, char** argv) -> void;
};

}
