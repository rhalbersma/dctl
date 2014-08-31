#pragma once

namespace dctl {
namespace bit {

template<class Block, int Nb>
class ConstIterator;

template<class Block>
class ConstIterator<Block, 1>;

}       // namespace bit
}       // namespace dctl
