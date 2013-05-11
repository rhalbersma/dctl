#pragma once
#include <algorithm>                    // find, find_if, generate_n, min_element, rotate, upper_bound
#include <iterator>                     // iterator_traits, iter_swap, next

namespace dctl {
namespace algorithm {

// non-mutating sequence algorithms

// runtime equivalent of boost::mpl::contains
// dctl::detect / std::count / std::find / std::copy
// O(N) complexity
template<typename InputIterator, typename EqualityComparable>
bool detect(InputIterator first, InputIterator last, EqualityComparable const& value)
{
        return std::find(first, last, value) != last;
}

// equivalent to std::any_of
// dctl::detect_if / std::count_if / std::find_if / std::copy_if
// O(N) complexity
template<typename InputIterator, typename Predicate>
bool detect_if(InputIterator first, InputIterator last, Predicate pred)
{
        return std::find_if(first, last, pred) != last;
}

// O(N) complexity
template<typename Container>
bool is_duplicate_back(Container const& c)
{
        return detect(c.begin(), c.end() - 1, c.back());
}

// mutating sequence algorithms

// O(N) complexity: weak range extension of std::iota
template<typename OutputIterator, typename Size, typename Assignable>
void iota_n(OutputIterator first, Size n, Assignable value)
{
        std::generate_n(first, n, [&]() { return value++; });
}

// sorting algorithms

// O(N^2) complexity
template< typename ForwardIterator, typename Compare = std::less<typename std::iterator_traits<ForwardIterator>::value_type> >
void insertion_sort(ForwardIterator first, ForwardIterator last, Compare cmp = Compare())
{
        for (auto it = first; it != last; ++it) {
                auto const insertion = std::upper_bound(first, it, *it, cmp);
                std::rotate(insertion, it, std::next(it));
        }
}

// O(N^2) complexity
template< typename ForwardIterator, typename Compare = std::less<typename std::iterator_traits<ForwardIterator>::value_type> >
void selection_sort(ForwardIterator first, ForwardIterator last, Compare cmp = Compare())
{
        for (auto it = first; it != last; ++it) {
                auto const selection = std::min_element(it, last, cmp);
                std::iter_swap(selection, it);
        }
}

}       // namespace algorithm
}       // namespace dctl
