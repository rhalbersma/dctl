#pragma once
#include <functional>

namespace dtl {
namespace hash {
namespace zobrist {

template<typename Key, typename Index>
struct Find: public std::unary_function<Key, Index>
{
        Index operator()(const Key&) const;
};

}       // namespace zobrist
}       // namespace hash
}       // namespace dtl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Find.hpp"
