#include <Interpreter.hpp>

int main(const int argc, char** argv)
{
  const auto interpreter = pinky::Interpreter(argc, argv);

  const auto result = interpreter.run();

  return result;
}
