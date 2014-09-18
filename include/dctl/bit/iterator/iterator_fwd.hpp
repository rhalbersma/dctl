#pragma once

namespace dctl {
namespace bit {

template<class Block, int Nb, int N>
class ConstIterator;

template<class Block, int N>
class ConstIterator<Block, 1, N>;

}       // namespace bit
}       // namespace dctl
