#pragma once
#include <functional>

namespace Hash {
namespace Jenkins {

template<typename Key, typename Index>
struct Init: public std::unary_function<Key, Index>
{
        Index operator()(const Key&) const;
};

}       // namespace Jenkins
}       // namespace Hash

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Init.hpp"
