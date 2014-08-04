#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace bit {

template<class T, class Block, std::size_t Nb>
class ConstIterator;

template<class T, class Block>
class ConstIterator<T, Block, 1>;

}       // namespace bit
}       // namespace dctl
