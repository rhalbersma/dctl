#pragma once
#include <algorithm>                    // for_each, generate_n, min_element, rotate, upper_bound
#include <iterator>                     // iter_swap, next

namespace dctl {

template<class OutputIterator, class Size, class Assignable>
void iota_n(OutputIterator first, Size n, Assignable value)
{
        std::generate_n(first, n, [&value]() {
                return value++;
        });
}

template<class ForwardIterator>
void insertion_sort(ForwardIterator first, ForwardIterator last)
{
        std::for_each(first, last, [](ForwardIterator it) {
                auto const insertion = std::upper_bound(first, it, *it);
                std::rotate(insertion, it, std::next(it));
        });
}

template<class ForwardIterator>
void selection_sort(ForwardIterator first, ForwardIterator last)
{
        std::for_each(first, last, [](ForwardIterator it) {
                auto const selection = std::min_element(it, last);
                std::iter_swap(selection, it);
        });
}

}       // namespace dctl
