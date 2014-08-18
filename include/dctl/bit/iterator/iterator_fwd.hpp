#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace bit {

template<class Block, std::size_t Nb>
class ConstIterator;

template<class Block>
class ConstIterator<Block, 1>;

}       // namespace bit
}       // namespace dctl
