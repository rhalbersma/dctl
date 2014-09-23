#pragma once

namespace xstd {

template<class Block, int Nb, int N>
class ConstIterator;

template<class Block, int N>
class ConstIterator<Block, 1, N>;

}       // namespace xstd
