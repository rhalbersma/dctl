#include <cassert>

template<typename T, size_t N>
typename Bucket<T, N>::iterator Bucket<T, N>::begin(void)
{
        return d_bucket;
}

template<typename T, size_t N>
typename Bucket<T, N>::const_iterator Bucket<T, N>::begin(void) const
{
        return d_bucket;
}

template<typename T, size_t N>
typename Bucket<T, N>::iterator Bucket<T, N>::end(void)
{
        return begin() + capacity();
}

template<typename T, size_t N>
typename Bucket<T, N>::const_iterator Bucket<T, N>::end(void) const
{
        return begin() + capacity();
}

template<typename T, size_t N>
typename Bucket<T, N>::reverse_iterator Bucket<T, N>::rbegin(void)
{
        return reverse_iterator(end());
}

template<typename T, size_t N>
typename Bucket<T, N>::const_reverse_iterator Bucket<T, N>::rbegin(void) const
{
        return const_reverse_iterator(end());
}

template<typename T, size_t N>
typename Bucket<T, N>::reverse_iterator Bucket<T, N>::rend(void)
{
        return reverse_iterator(begin());
}

template<typename T, size_t N>
typename Bucket<T, N>::const_reverse_iterator Bucket<T, N>::rend(void) const
{
        return const_reverse_iterator(begin());
}

template<typename T, size_t N>      
size_t Bucket<T, N>::capacity(void) const
{
        return N;
}

template<typename T, size_t N>
T& Bucket<T, N>::operator[](size_t i)
{
        assert(in_range(i));
        return *(begin() + i);
}

template<typename T, size_t N>
const T& Bucket<T, N>::operator[](size_t i) const
{
        assert(in_range(i));
        return *(begin() + i);
}

template<typename T, size_t N>
T& Bucket<T, N>::front(void)
{
        return *begin();
}

template<typename T, size_t N>
const T& Bucket<T, N>::front(void) const
{
        return *begin();
}

template<typename T, size_t N>
T& Bucket<T, N>::back(void)
{
        return *(end() - 1);
}

template<typename T, size_t N>
const T& Bucket<T, N>::back(void) const
{
        return *(end() - 1);
}

template<typename T, size_t N>
bool Bucket<T, N>::in_range(size_t i) const
{
        return i < capacity();
}
