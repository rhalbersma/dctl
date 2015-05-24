#pragma once
#include <algorithm>    // find, find_if, min_element, rotate, upper_bound
#include <iterator>     // iterator_traits, iter_swap, next, prev

namespace dctl {
namespace util {

// non-mutating sequence algorithms

// O(N) complexity
template
<
        class InputIterator, class T, class UnaryPredicate,
        class DifferenceType = typename std::iterator_traits<InputIterator>::difference_type
>
std::pair<InputIterator, DifferenceType>
count_until(InputIterator begin, InputIterator end, T const& value, UnaryPredicate pred)
{
        DifferenceType count = 0;
        auto const it = std::find_if(
                begin, end,
                [=, &value, &count](auto const& e) {
                return e == value && pred(++count);
        });
        return { it, count };
}

// O(N) complexity
template<class InputIterator, class T, class BinaryOperation, class UnaryPredicate>
std::pair<InputIterator, T>
accumulate_until(InputIterator begin, InputIterator end, T init, BinaryOperation op, UnaryPredicate pred)
{
        auto const it = std::find_if(
                begin, end,
                [=, &init](auto const& e) {
                init = op(init, e);
                return pred(init);
        });
        return { it, init };
}

// mutating sequence algorithms

// sorting algorithms

// O(N^2) complexity
template<class ForwardIterator, class Compare = std::less<>>
auto insertion_sort(ForwardIterator first, ForwardIterator last, Compare cmp = Compare{})
{
        for (auto it = first; it != last; ++it) {
                auto const insertion = std::upper_bound(first, it, *it, cmp);
                std::rotate(insertion, it, std::next(it));
        }
}

// O(N^2) complexity
template<class ForwardIterator, class Compare = std::less<>>
auto selection_sort(ForwardIterator first, ForwardIterator last, Compare cmp = Compare{})
{
        for (auto it = first; it != last; ++it) {
                auto const selection = std::min_element(it, last, cmp);
                std::iter_swap(selection, it);
        }
}

}       // namespace util
}       // namespace dctl
