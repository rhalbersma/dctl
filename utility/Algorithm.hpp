#pragma once
#include <algorithm>
#include <iterator>

namespace dctl {

template<class OutputIterator, class Size, class Assignable>
void iota_n (OutputIterator first, Size n, Assignable value)
{
        std::for_each(first, first + n, [=]() { 
                *first++ = value++;
        });
}

template<class ForwardIterator>
void insertion_sort_dispatch(ForwardIterator first, ForwardIterator last)
{    
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        detail::insertion_sort_dispatch(first, last, category());
}

template<class ForwardIterator, class StrictWeakOrdering>
void insertion_sort_dispatch(ForwardIterator first, ForwardIterator last, StrictWeakOrdering comp)
{    
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        detail::insertion_sort_dispatch(first, last, comp, category());
}

template<class ForwardIterator>
void selection_sort_dispatch(ForwardIterator first, ForwardIterator last)
{    
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        detail::selection_sort_dispatch(first, last, category());
}

template<class ForwardIterator, class StrictWeakOrdering>
void selection_sort_dispatch(ForwardIterator first, ForwardIterator last, StrictWeakOrdering comp)
{    
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        detail::selection_sort_dispatch(first, last, comp, category());
}

namespace detail {

template<class ForwardIterator>
void insertion_sort_dispatch(ForwardIterator first, ForwardIterator last, std::forward_iterator_tag)
{    
        std::for_each(first, last, [=](ForwardIterator it) {                    
                insertion_sorter(first, it);
        });
}

template<class RandomAccessIterator>
void insertion_sort_dispatch(RandomAccessIterator first, RandomAccessIterator last, std::random_access_iterator_tag)
{    
        for (auto it = first + 1; it < last; ++it) {                    
                insertion_sorter(first, it);
        }
}

template<class ForwardIterator, class StrictWeakOrdering>
void insertion_sort_dispatch(ForwardIterator first, ForwardIterator last, StrictWeakOrdering comp, std::forward_iterator_tag)
{    
        std::for_each(first, last, [=](ForwardIterator it) {                    
                insertion_sorter(first, it, comp);
        });
}

template<class RandomAccessIterator, class StrictWeakOrdering>
void insertion_sort_dispatch(RandomAccessIterator first, RandomAccessIterator last, StrictWeakOrdering comp, std::random_access_iterator_tag)
{    
        for (auto it = first + 1; it < last; ++it) {                    
                insertion_sorter(first, it, comp);
        }
}

template<class ForwardIterator>
void selection_sort_dispatch(ForwardIterator first, ForwardIterator last, std::forward_iterator_tag)
{    
        std::for_each(first, last, [=](ForwardIterator it) {                    
                selection_sorter(it, last);
        });
}

template<class RandomAccessIterator>
void selection_sort_dispatch(RandomAccessIterator first, RandomAccessIterator last, std::random_access_iterator_tag)
{    
        for (; first < last - 1; ++first) {                    
                selection_sorter(first, last);
        }
}

template<class ForwardIterator, class StrictWeakOrdering>
void selection_sort_dispatch(ForwardIterator first, ForwardIterator last, StrictWeakOrdering comp, std::forward_iterator_tag)
{    
        std::for_each(first, last, [=](ForwardIterator it) {                    
                selection_sorter(it, last, comp);
        });
}

template<class RandomAccessIterator, class StrictWeakOrdering>
void selection_sort_dispatch(RandomAccessIterator first, RandomAccessIterator last, StrictWeakOrdering comp, std::random_access_iterator_tag)
{    
        for (; first < last - 1; ++first) {                    
                selection_sorter(first, last, comp);
        }
}

template<class ForwardIterator>
void insertion_sorter(ForwardIterator first, ForwardIterator last)
{    
        std::rotate(std::upper_bound(first, last, *last), last, last + 1);
}

template<class ForwardIterator, class StrictWeakOrdering>
void insertion_sorter(ForwardIterator first, ForwardIterator last, StrictWeakOrdering comp)
{    
        std::rotate(std::upper_bound(first, last, *last, comp), last, last + 1);
}

template<class ForwardIterator>
void selection_sorter(ForwardIterator first, ForwardIterator last)
{    
        std::iter_swap(std::min_element(first, last), first);
}

template<class ForwardIterator, class StrictWeakOrdering>
void selection_sorter(ForwardIterator first, ForwardIterator last, StrictWeakOrdering comp)
{    
        std::iter_swap(std::min_element(first, last, comp), first);
}

}       // namespace detail

}       // namespace dctl
