#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace bit {
namespace detail {

template<class Key, class Compare, class Block, std::size_t Nb>
struct BaseSet;

template<class Key, class Compare, class Block>
struct BaseSet<Key, Compare, Block, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
