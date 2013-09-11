#pragma once

namespace dctl {
namespace bit {

template<int, class = void> class bit_reference;
template<class _> class bit_reference<1, _>;

}       // namespace bit
}       // namespace dctl
