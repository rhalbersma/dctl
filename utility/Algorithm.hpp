#pragma once
#include <algorithm>                    // min_element, rotate, upper_bound
#include <iterator>                     // iter_swap, next

namespace dctl {

template<class OutputIterator, class Size, class Assignable>
void iota_n(OutputIterator first, Size n, Assignable value)
{
        for (Size i = 0; i != n; ++i)
                *first++ = value++;
}

template<class ForwardIterator>
void insertion_sort(ForwardIterator first, ForwardIterator last)
{
        std::for_each(first, last, [](ForwardIterator it) {
                const auto insertion = std::upper_bound(first, it, *it);
                std::rotate(insertion, it, std::next(it));
        });
}

template<class ForwardIterator>
void selection_sort(ForwardIterator first, ForwardIterator last)
{
        std::for_each(first, last, [](ForwardIterator it) {
                const auto selection = std::min_element(it, last);
                std::iter_swap(selection, it);
        });
}

}       // namespace dctl
