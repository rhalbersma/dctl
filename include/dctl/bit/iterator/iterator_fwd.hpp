#pragma once

namespace dctl {
namespace bit {

template<class T, class Block, int Nb>
class Iterator;

template<class T, class Block>
class Iterator<T, Block, 1>;

}       // namespace bit
}       // namespace dctl
