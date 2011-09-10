#pragma once
#include <string>       // std::string

namespace dctl {

class Material;

namespace setup {

template<typename> std::string content(const Material&, int);

}       // namespace setup
}       // namespace dctl

// include template definitions inside header
#include "Content.hpp"
