
#pragma once

#include <sstream>

namespace decibel
{
template <typename T>
std::string ToString(const T& val)
{
    std::stringstream out;
    out << val;
    return out.str();
}
}
