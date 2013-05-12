#pragma once
#include <type_traits>                          // enable_if, is_unsigned
#include <dctl/bit/bit.hpp>                     // count
#include <dctl/packed/iterator.hpp>             // bit_iterator

namespace dctl {
namespace packed {

//      begin()/end() for iteration over builtin unsigned integral types.
//
//      Note that C++11 ranged-for loop applies argument-dependent-lookup,
//      with std as associated namespace, finding std::begin() and std::end()
//      for containers with corresponding member functions and for C-arrays.
//      Unfortunately, adding overloads or specializations for builtin types to
//      namespace std leads to undefined behavior.
//      This rules out the C++11 ranged-for loop construct for builtin types.

template<class U, class Requires = typename std::enable_if<std::is_unsigned<U>::value>::type>
auto begin(U u) -> decltype(bit_iterator<int, U>(u))
{
        return bit_iterator<int, U>(u);
}

template<class U, class Requires = typename std::enable_if<std::is_unsigned<U>::value>::type>
auto end(U /* u */) -> decltype(bit_iterator<int, U>())
{
        return bit_iterator<int, U>();
}

template<class U, class Requires = typename std::enable_if<std::is_unsigned<U>::value>::type>
auto empty(U u) -> decltype(begin(u) == end(u))
{
        return begin(u) == end(u);
}

template<class U, class Requires = typename std::enable_if<std::is_unsigned<U>::value>::type>
auto size(U u) -> decltype(bit::count(u))
{
        return bit::count(u);
}

}       // namespace packed
}       // namespace dctl
