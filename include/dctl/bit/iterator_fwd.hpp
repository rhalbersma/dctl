#pragma once
#include <cstdint>                      // uint64_t
#include <type_traits>                  // enable_if, is_integral, is_unsigned

namespace dctl {
namespace bit {

template<class T, class U = uint64_t, class Requires = typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<U>::value>::type> class bit_iterator;
template<class T, class U = uint64_t, class Requires = typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<U>::value>::type> class bit_reference;

}       // namespace bit
}       // namespace dctl
