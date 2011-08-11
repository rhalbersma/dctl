#pragma once
#include <string>       // std::string

namespace dctl {

class Material;

namespace setup {

template<typename> std::string content(const Material&, int);

}       // namespace setup
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Content.hpp"
