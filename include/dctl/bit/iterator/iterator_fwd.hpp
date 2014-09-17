#pragma once

namespace dctl {
namespace bit {

template<int N, int Nb>
class ConstIterator;

template<int N>
class ConstIterator<N, 1>;

}       // namespace bit
}       // namespace dctl
