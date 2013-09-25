#pragma once

namespace dctl {
namespace bit {
namespace detail {

template<class Block, int Nb>
struct base_iterator;

template<class Block>
struct base_iterator<Block, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
