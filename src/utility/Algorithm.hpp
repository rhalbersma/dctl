#pragma once
#include <algorithm>                    // min_element, rotate, upper_bound
#include <iterator>                     // iter_swap, next
#include <tuple>

namespace dctl {

template<class OutputIterator, class Size, class Assignable>
void iota_n(OutputIterator first, Size n, Assignable value)
{
        for (Size i = 0; i != n; ++i)
                *first++ = value++;
}

// VC+ does not have is_permutation; Boost.Algoritm version gives spurious C4512 warnings
// http://en.cppreference.com/w/cpp/algorithm/is_permutation

template<class ForwardIterator1, class ForwardIterator2>
bool is_permutation(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 d_first)
{
        // skip common prefix
        std::tie(first, d_first) = std::mismatch(first, last, d_first);
        
        // iterate over the rest, counting how many times each element
        // from [first, last) appears in [d_first, d_last)
        if (first != last) {
                ForwardIterator2 d_last = d_first;
                std::advance(d_last, std::distance(first, last));
                
                for (ForwardIterator1 i = first; i != last; ++i) {
                        if (i != std::find(first, i, *i)) continue; // already counted this *i
 
                        auto m = std::count(d_first, d_last, *i);
                        if (m == 0 || std::count(i, last, *i) != m) {
                                return false;
                        }
                }       
        }
        return true;
}

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
bool is_permutation(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 d_first, BinaryPredicate p)
{
        // skip common prefix
        std::tie(first, d_first) = std::mismatch(first, last, d_first, p);
        
        // iterate over the rest, counting how many times each element
        // from [first, last) appears in [d_first, d_last)
        if (first != last) {
                ForwardIterator2 d_last = d_first;
                std::advance(d_last, std::distance(first, last));
                
                for (ForwardIterator1 i = first; i != last; ++i) {
                        if (i != std::find_if(first, i, *i, p)) continue; // already counted this *i
 
                        auto m = std::count_if(d_first, d_last, *i, p);
                        if (m == 0 || std::count_if(i, last, *i, p) != m) {
                                return false;
                        }
                }       
        }
        return true;
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
