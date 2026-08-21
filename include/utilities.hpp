#pragma once

#include <string>
#include <istream>


namespace utilities
{
auto readStream(std::istream& is) -> std::string;
auto readTextFile(const std::string& filename) -> std::string;
}
