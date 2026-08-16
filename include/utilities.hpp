#pragma once

#include <string>


namespace utilities
{
auto readStream(std::istream& is) -> std::string;
auto readTextFile(const std::string& filename) -> std::string;
}
