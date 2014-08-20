#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace bit {
namespace detail {

template<class Block, std::size_t Nb>
class BaseSet;

template<class Block>
class BaseSet<Block, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
