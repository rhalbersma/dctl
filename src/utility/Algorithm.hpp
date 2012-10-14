#pragma once
#include <algorithm>                    // find, for_each, generate_n, min_element, rotate, upper_bound
#include <iterator>                     // iter_swap, next

namespace dctl {
namespace algorithm {

// non-mutating sequence algorithms

template<typename InputIterator, typename EqualityComparable>
bool contains(InputIterator first, InputIterator last, EqualityComparable const& value)
{
        return std::find(first, last, value) != last;
}

// mutating sequence algorithms

template<typename OutputIterator, typename Size, typename Assignable>
void iota_n(OutputIterator first, Size n, Assignable value)
{
        std::generate_n(first, n, [&value]() {
                return value++;
        });
}

// sorting

template<typename ForwardIterator>
void insertion_sort(ForwardIterator first, ForwardIterator last)
{
        for(auto it = first; it != last; ++it) {
                auto const insertion = std::upper_bound(first, it, *it);
                std::rotate(insertion, it, std::next(it));
        }
}

template<typename ForwardIterator>
void selection_sort(ForwardIterator first, ForwardIterator last)
{
        for(auto it = first; it != last; ++it) {
                auto const selection = std::min_element(it, last);
                std::iter_swap(selection, it);
        }
}

}       // namespace algorithm
}       // namespace dctl
