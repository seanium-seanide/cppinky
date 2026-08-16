#pragma once


namespace cppinky
{

class Interpreter
{
public:
  Interpreter() = default;
  Interpreter(int argc, char**argv);

  int run();
};

}
