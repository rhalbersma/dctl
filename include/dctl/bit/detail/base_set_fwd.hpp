#pragma once

namespace dctl {
namespace bit {
namespace detail {

template<class Block, int Nb>
class BaseSet;

template<class Block>
class BaseSet<Block, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
