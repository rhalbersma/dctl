#pragma once

namespace dctl {
namespace bit {
namespace detail {

template<class T, class Block, int Nw>
struct BaseSet;

template<class T, class Block>
struct BaseSet<T, Block, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
