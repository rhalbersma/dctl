#pragma once
#include <limits>       // digits, is_signed, is_integer

namespace dctl {
namespace bit {

template<class T> constexpr auto none =  static_cast<T>(0);
template<class T> constexpr auto one  =  static_cast<T>(1);
template<class T> constexpr auto all  = ~static_cast<T>(0);

template<class T>
constexpr auto digits = std::numeric_limits<T>::digits;

template<class T>
constexpr auto is_unsigned_integer =
        !std::numeric_limits<T>::is_signed &&
         std::numeric_limits<T>::is_integer
;

}       // namespace bit
}       // namespace dctl
