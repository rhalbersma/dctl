#pragma once
#include <type_traits>                          // enable_if, is_unsigned
#include <dctl/bit/bit.hpp>                     // count
#include <dctl/packed/iterator.hpp>             // bit_iterator

namespace dctl {
namespace packed {

/*
      begin()/end() for iteration over builtin unsigned integral types.

      NOTE: a C++11 ranged-based for() loop applies argument-dependent-lookup
      with std as associated namespace, finding std::begin() and std::end(),
      which will delegate to member functions begin()/end() where appropriate,
      as well as to overloads for C-style arrays.

      Unfortunately, the builtin types neither have any asssociated namespaces,
      nor is ordinary unqualified name lookup being applied. This means that
      the functions begin()/end() defined below for builtin types will NOT be
      found by a C++11 ranged-based for() loop.

      Furthermore, adding overloads or specializations for builtin types to
      namespace std (where they would be found) leads to undefined behavior.

      This rules out a C++11 ranged-based for() loop for builtin types.
*/

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
