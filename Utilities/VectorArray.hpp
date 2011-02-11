#include <cassert>

template<typename T, size_t N>
VectorArray<T, N>::VectorArray(void)
:
        size_(0)
{
        assert(invariant());
}

template<typename T, size_t N>
VectorArray<T, N>::VectorArray(size_type s)
:
        size_(s)
{
        assert(invariant());
}

template<typename T, size_t N>
typename VectorArray<T, N>::iterator VectorArray<T, N>::begin(void)
{
        return array_.begin();
}

template<typename T, size_t N>
typename VectorArray<T, N>::const_iterator VectorArray<T, N>::begin(void) const
{
        return array_.begin();
}

template<typename T, size_t N>
typename VectorArray<T, N>::iterator VectorArray<T, N>::end(void)
{
        return begin() + size();
}

template<typename T, size_t N>
typename VectorArray<T, N>::const_iterator VectorArray<T, N>::end(void) const
{
        return begin() + size();
}

template<typename T, size_t N>
typename VectorArray<T, N>::reverse_iterator VectorArray<T, N>::rbegin(void)
{
        return reverse_iterator(end());
}

template<typename T, size_t N>
typename VectorArray<T, N>::const_reverse_iterator VectorArray<T, N>::rbegin(void) const
{
        return const_reverse_iterator(end());
}

template<typename T, size_t N>
typename VectorArray<T, N>::reverse_iterator VectorArray<T, N>::rend(void)
{
        return array_.rend();
}

template<typename T, size_t N>
typename VectorArray<T, N>::const_reverse_iterator VectorArray<T, N>::rend(void) const
{
        return array_.rend();
}

template<typename T, size_t N>
typename VectorArray<T, N>::size_type VectorArray<T, N>::size(void) const
{
        return size_;
}

template<typename T, size_t N>
void VectorArray<T, N>::resize(size_type s)
{
        assert(within_bounds(s));
        size_ = s;
}

template<typename T, size_t N>
typename VectorArray<T, N>::size_type VectorArray<T, N>::capacity(void) const
{
        return array_.size();
}

template<typename T, size_t N>
bool VectorArray<T, N>::empty(void) const
{
        return begin() == end();
}

template<typename T, size_t N>
bool VectorArray<T, N>::full(void) const
{
        return size() == capacity();
}

template<typename T, size_t N>
typename VectorArray<T, N>::reference VectorArray<T, N>::operator[](size_type i)
{
        assert(within_range(i));
        return array_[i];
}

template<typename T, size_t N>
typename VectorArray<T, N>::const_reference VectorArray<T, N>::operator[](size_type i) const
{
        assert(within_range(i));
        return array_[i];
}

template<typename T, size_t N>
typename VectorArray<T, N>::reference VectorArray<T, N>::back(void)
{
        return *(end() - 1);
}

template<typename T, size_t N>
typename VectorArray<T, N>::const_reference VectorArray<T, N>::back(void) const
{
        return *(end() - 1);
}

template<typename T, size_t N>
void VectorArray<T, N>::copy_back(void)
{
        assert(!empty());
        push_back(back());
}

template<typename T, size_t N>
void VectorArray<T, N>::push_back(const_reference t)
{
        push();
        back() = t;
}

template<typename T, size_t N>
void VectorArray<T, N>::push(void)
{
        assert(!full());
        ++size_;
        assert(invariant());
}

template<typename T, size_t N>
void VectorArray<T, N>::pop_back(void)
{
        assert(!empty());
        --size_;
        assert(invariant());
}

template<typename T, size_t N>
void VectorArray<T, N>::clear(void)
{
        size_ = 0;
        assert(invariant());
}

template<typename T, size_t N>
bool VectorArray<T, N>::invariant(void) const
{
        return size() <= capacity();
}

template<typename T, size_t N>
bool VectorArray<T, N>::within_range(size_type i) const
{
        return i < size();
}

template<typename T, size_t N>
bool VectorArray<T, N>::within_bounds(size_type i) const
{
        return i < capacity();
}
