#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace bit {

template<class UnsignedInteger, std::size_t Nb>
class ConstIterator;

template<class UnsignedInteger>
class ConstIterator<UnsignedInteger, 1>;

}       // namespace bit
}       // namespace dctl
