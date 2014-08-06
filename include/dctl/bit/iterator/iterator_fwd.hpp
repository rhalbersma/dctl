#pragma once

namespace dctl {
namespace bit {

template<class T, class Block, int Nb>
class ConstIterator;

template<class T, class Block>
class ConstIterator<T, Block, 1>;

}       // namespace bit
}       // namespace dctl
