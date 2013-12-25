#pragma once

namespace dctl {
namespace bit {
namespace detail {

template<class Block, int Nb>
class base_iterator;

template<class Block>
class base_iterator<Block, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
