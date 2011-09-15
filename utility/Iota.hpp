#pragma once

namespace dctl {
namespace utility {

/*

        The C++0x draft contains a function template iota:

        template <class ForwardIterator, class T>
        void iota(ForwardIterator first, ForwardIterator last, T value);

        In contrast to the pairs fill/fill_n, successor/generate_n and 
        search/searn_n, the C++0x draft does not contain a corresponding 
        function template iota_n.

*/

template <class ForwardIterator, class Size, class T>
void iota_n (ForwardIterator first, Size n, T value)
{
        for (Size i = 0; i < n; ++i)
                *first++ = value++;
}

}       // namespace utility
}       // namespace dctl
