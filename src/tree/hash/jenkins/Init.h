#pragma once
#include <functional>

namespace tree {
namespace hash {
namespace jenkins {

template<typename Key, typename Index>
struct Init: public std::unary_function<Key, Index>
{
        Index operator()(const Key&) const;
};

}       // namespace jenkins
}       // namespace hash
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Init.hpp"
