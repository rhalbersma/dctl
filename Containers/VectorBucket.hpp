#include <cassert>

template<typename T, size_t N>
VectorBucket<T, N>::VectorBucket(void)
:
        size_(0)
{
        assert(invariant());
}

template<typename T, size_t N>
VectorBucket<T, N>::VectorBucket(size_t s)
:
        size_(s)
{
        assert(invariant());
}

template<typename T, size_t N>
typename VectorBucket<T, N>::iterator VectorBucket<T, N>::begin(void)
{
        return bucket_.begin();
}

template<typename T, size_t N>
typename VectorBucket<T, N>::const_iterator VectorBucket<T, N>::begin(void) const
{
        return bucket_.begin();
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
        return bucket_.rend();
}

template<typename T, size_t N>
typename VectorBucket<T, N>::const_reverse_iterator VectorBucket<T, N>::rend(void) const
{
        return bucket_.rend();
}

template<typename T, size_t N>
size_t VectorBucket<T, N>::size(void) const
{
        return size_;
}

template<typename T, size_t N>
void VectorBucket<T, N>::resize(size_t s)
{
        assert(within_bounds(s));
        size_ = s;
}

template<typename T, size_t N>
size_t VectorBucket<T, N>::capacity(void) const
{
        return bucket_.size();
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
typename VectorBucket<T, N>::reference VectorBucket<T, N>::operator[](size_t i)
{
        assert(within_range(i));
        return bucket_[i];
}

template<typename T, size_t N>
typename VectorBucket<T, N>::const_reference VectorBucket<T, N>::operator[](size_t i) const
{
        assert(within_range(i));
        return bucket_[i];
}

template<typename T, size_t N>
typename VectorBucket<T, N>::reference VectorBucket<T, N>::back(void)
{
        return *(end() - 1);
}

template<typename T, size_t N>
typename VectorBucket<T, N>::const_reference VectorBucket<T, N>::back(void) const
{
        return *(end() - 1);
}

template<typename T, size_t N>
void VectorBucket<T, N>::copy_back(void)
{
        assert(!empty());
        push_back(back());
}

template<typename T, size_t N>
void VectorBucket<T, N>::push_back(const_reference t)
{
        push();
        back() = t;
}

template<typename T, size_t N>
void VectorBucket<T, N>::push(void)
{
        assert(!full());
        ++size_;
        assert(invariant());
}

template<typename T, size_t N>
void VectorBucket<T, N>::pop_back(void)
{
        assert(!empty());
        --size_;
        assert(invariant());
}

template<typename T, size_t N>
void VectorBucket<T, N>::clear(void)
{
        size_ = 0;
        assert(invariant());
}

template<typename T, size_t N>
bool VectorBucket<T, N>::invariant(void) const
{
        return size() <= capacity();
}

template<typename T, size_t N>
bool VectorBucket<T, N>::within_range(size_t i) const
{
        return i < size();
}

template<typename T, size_t N>
bool VectorBucket<T, N>::within_bounds(size_t i) const
{
        return i < capacity();
}
