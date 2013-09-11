#pragma once

namespace dctl {
namespace bit {

template<int, class = void> class bit_iterator;
template<class _> class bit_iterator<1, _>;

}       // namespace bit
}       // namespace dctl
