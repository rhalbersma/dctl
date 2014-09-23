#pragma once
#include <limits>       // digits, is_signed, is_integer

namespace xstd {

template<class T>
constexpr auto digits = std::numeric_limits<T>::digits;

template<class T>
constexpr auto is_unsigned_integer =
        !std::numeric_limits<T>::is_signed &&
         std::numeric_limits<T>::is_integer
;

}       // namespace xstd
