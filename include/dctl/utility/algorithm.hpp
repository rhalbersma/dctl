#pragma once
#include <algorithm>                    // find, find_if, generate_n, min_element, rotate, upper_bound
#include <iterator>                     // iterator_traits, iter_swap, next, prev

namespace dctl {
namespace util {

// non-mutating sequence algorithms

// runtime equivalent of Boost.MPL contains()
// O(N) complexity
template<class InputIterator>
bool detect(InputIterator first, InputIterator last, typename std::iterator_traits<InputIterator>::value_type const& value)
{
        return std::find(first, last, value) != last;
}

// equivalent to std::any_of
// O(N) complexity
template<class InputIterator, class Predicate>
bool detect_if(InputIterator first, InputIterator last, Predicate pred)
{
        return std::find_if(first, last, pred) != last;
}

// O(N) complexity
template<class BidirectionalSequenceContainer>
bool is_duplicate_back(BidirectionalSequenceContainer const& c)
{
        return util::detect(begin(c), std::prev(end(c)), c.back());
}

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

// O(N) complexity: weak range extension of std::iota
template<class OutputIterator, class Size, class T>
void iota_n(OutputIterator first, Size n, T value)
{
        std::generate_n(first, n, [&]() { return value++; });
}

// sorting algorithms

// O(N^2) complexity
template<class ForwardIterator, class Compare = std::less<typename std::iterator_traits<ForwardIterator>::value_type>>
void insertion_sort(ForwardIterator first, ForwardIterator last, Compare cmp = Compare{})
{
        for (auto it = first; it != last; ++it) {
                auto const insertion = std::upper_bound(first, it, *it, cmp);
                std::rotate(insertion, it, std::next(it));
        }
}

// O(N^2) complexity
template<class ForwardIterator, class Compare = std::less<typename std::iterator_traits<ForwardIterator>::value_type>>
void selection_sort(ForwardIterator first, ForwardIterator last, Compare cmp = Compare{})
{
        for (auto it = first; it != last; ++it) {
                auto const selection = std::min_element(it, last, cmp);
                std::iter_swap(selection, it);
        }
}

}       // namespace util
}       // namespace dctl
