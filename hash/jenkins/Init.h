#pragma once
#include <functional>

namespace dctl {
namespace hash {
namespace jenkins {

template<typename Key, typename Index>
struct Init
: 
        public std::unary_function<Key, Index>
{
        Index operator()(const Key&) const;
};

}       // namespace jenkins
}       // namespace hash
}       // namespace dctl

// include template definitions inside header
#include "Init.hpp"
