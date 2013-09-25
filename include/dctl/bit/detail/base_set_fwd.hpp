#pragma once

namespace dctl {
namespace bit {
namespace detail {

template<class T, class Block, int Nw>
struct base_set;

template<class T, class Block>
struct base_set<T, Block, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
