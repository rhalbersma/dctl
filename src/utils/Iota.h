#pragma once

namespace dctl {

/*

The C++0x draft contains a function template iota:

template <class ForwardIterator, class T>
void iota(ForwardIterator first, ForwardIterator last, T value);

In contrast to the pairs fill/fill_n, generate/generate_n and search/searn_n,
the C++0x draft does not contain a corresponding function template iota_n

*/

template <class ForwardIterator, class Size, class T>
void iota_n(ForwardIterator first, Size n, T value);

}     // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Iota.hpp"
