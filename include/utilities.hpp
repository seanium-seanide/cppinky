#pragma once

#include <string>
#include <istream>
#include <TokenizerTypes.hpp>


namespace utilities
{
auto readStream(std::istream& is) -> std::string;
auto readTextFile(const std::string& filename) -> std::string;
auto isdigit(pinky::char_type c) -> bool;
auto isalpha(pinky::char_type c) -> bool;
auto isspace(pinky::char_type c) -> bool;
auto isalnum(pinky::char_type c) -> bool;
}
