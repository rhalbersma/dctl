#pragma once

namespace dctl {
namespace bit {
namespace detail {

template<class Key, class Compare, class Block, int Nb>
struct BaseSet;

template<class Key, class Compare, class Block>
struct BaseSet<Key, Compare, Block, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
