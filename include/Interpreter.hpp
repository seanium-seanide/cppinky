#pragma once

#include <string>


namespace cppinky
{

class Interpreter
{
public:
  Interpreter() = default;
  Interpreter(int argc, char**argv);

  int run();

private:
  std::string m_scriptFileName;

  void parseCommandLineArgs(int argc, char** argv);
};

}
