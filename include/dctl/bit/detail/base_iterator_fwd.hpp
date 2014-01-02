#pragma once

namespace dctl {
namespace bit {
namespace detail {

template<class Block, int Nb>
struct BaseIterator;

template<class Block>
struct BaseIterator<Block, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
