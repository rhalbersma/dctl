#include <cassert>

template<typename T, size_t N>
VectorBucket<T, N>::VectorBucket(void)
:
        d_size(0)
{
        assert(invariant());
}

template<typename T, size_t N>
VectorBucket<T, N>::VectorBucket(size_t s_size)
:
        d_size(s_size)
{
        assert(invariant());
}

template<typename T, size_t N>
typename VectorBucket<T, N>::iterator VectorBucket<T, N>::begin(void)
{
        return d_bucket.begin();
}

template<typename T, size_t N>
typename VectorBucket<T, N>::const_iterator VectorBucket<T, N>::begin(void) const
{
        return d_bucket.begin();
}

template<typename T, size_t N>
typename VectorBucket<T, N>::iterator VectorBucket<T, N>::end(void)
{
        return begin() + size();
}

template<typename T, size_t N>
typename VectorBucket<T, N>::const_iterator VectorBucket<T, N>::end(void) const
{
        return begin() + size();
}

template<typename T, size_t N>
typename VectorBucket<T, N>::reverse_iterator VectorBucket<T, N>::rbegin(void)
{
        return reverse_iterator(end());
}

template<typename T, size_t N>
typename VectorBucket<T, N>::const_reverse_iterator VectorBucket<T, N>::rbegin(void) const
{
        return const_reverse_iterator(end());
}

template<typename T, size_t N>
typename VectorBucket<T, N>::reverse_iterator VectorBucket<T, N>::rend(void)
{
        return reverse_iterator(begin());
}

template<typename T, size_t N>
typename VectorBucket<T, N>::const_reverse_iterator VectorBucket<T, N>::rend(void) const
{
        return const_reverse_iterator(begin());
}

template<typename T, size_t N>
size_t VectorBucket<T, N>::size(void) const
{
        return d_size;
}

template<typename T, size_t N>
void VectorBucket<T, N>::resize(size_t s_size)
{
        d_size = s_size;
}

template<typename T, size_t N>
size_t VectorBucket<T, N>::capacity(void) const
{
        return d_bucket.capacity();
}

template<typename T, size_t N>
bool VectorBucket<T, N>::empty(void) const
{
        return begin() == end();
}

template<typename T, size_t N>
bool VectorBucket<T, N>::full(void) const
{
        return size() == capacity();
}

template<typename T, size_t N>
T& VectorBucket<T, N>::operator[](size_t i)
{
        assert(in_range(i));
        return *(begin() + i);
}

template<typename T, size_t N>
const T& VectorBucket<T, N>::operator[](size_t i) const
{
        assert(in_range(i));
        return *(begin() + i);
}

template<typename T, size_t N>
T& VectorBucket<T, N>::back(void)
{
        return *(end() - 1);
}

template<typename T, size_t N>
const T& VectorBucket<T, N>::back(void) const
{
        return *(end() - 1);
}

template<typename T, size_t N>
void VectorBucket<T, N>::copy_back(void)
{
        assert(!empty());       // pre-condition
        push_back(back());
}

template<typename T, size_t N>
void VectorBucket<T, N>::push_back(const T& t)
{
        push();
        back() = t;
}

template<typename T, size_t N>
void VectorBucket<T, N>::push(void)
{
        assert(!full());        // pre-condition
        ++d_size;
        assert(invariant());
}

template<typename T, size_t N>
void VectorBucket<T, N>::pop_back(void)
{
        assert(!empty());       // pre-condition
        --d_size;
        assert(invariant());
}

template<typename T, size_t N>
void VectorBucket<T, N>::clear(void)
{
        d_size = 0;
        assert(invariant());
}

template<typename T, size_t N>
bool VectorBucket<T, N>::invariant(void) const
{
        return size() <= capacity();
}

template<typename T, size_t N>
bool VectorBucket<T, N>::in_range(size_t i) const
{
        return i < size();
}
