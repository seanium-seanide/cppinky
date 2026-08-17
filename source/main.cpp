#include <Interpreter.hpp>

int main(int argc, char** argv)
{
  auto interpreter = pinky::Interpreter(argc, argv);

  auto result = interpreter.run();

  return result;
}
