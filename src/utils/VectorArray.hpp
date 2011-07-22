#include <cassert>

namespace dctl {

template<typename T, std::size_t N>
VectorArray<T, N>::VectorArray()
:
        size_(0)
{
        assert(invariant());
}

template<typename T, std::size_t N>
VectorArray<T, N>::VectorArray(size_type s)
:
        size_(s)
{
        assert(invariant());
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::iterator VectorArray<T, N>::begin()
{
        return array_.begin();
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::const_iterator VectorArray<T, N>::begin() const
{
        return array_.begin();
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::iterator VectorArray<T, N>::end()
{
        return begin() + size();
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::const_iterator VectorArray<T, N>::end() const
{
        return begin() + size();
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::reverse_iterator VectorArray<T, N>::rbegin()
{
        return reverse_iterator(end());
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::const_reverse_iterator VectorArray<T, N>::rbegin() const
{
        return const_reverse_iterator(end());
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::reverse_iterator VectorArray<T, N>::rend()
{
        return array_.rend();
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::const_reverse_iterator VectorArray<T, N>::rend() const
{
        return array_.rend();
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::size_type VectorArray<T, N>::size() const
{
        return size_;
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::size_type VectorArray<T, N>::max_size() const
{
        return N;
}
template<typename T, std::size_t N>
void VectorArray<T, N>::resize(size_type s)
{
        assert(within_bounds(s));
        size_ = s;
        assert(invariant());
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::size_type VectorArray<T, N>::capacity() const
{
        return size();
}

template<typename T, std::size_t N>
bool VectorArray<T, N>::empty() const
{
        return begin() == end();
}

template<typename T, std::size_t N>
bool VectorArray<T, N>::full() const
{
        return size() == max_size();
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::reference VectorArray<T, N>::operator[](size_type i)
{
        assert(within_range(i));
        return array_[i];
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::const_reference VectorArray<T, N>::operator[](size_type i) const
{
        assert(within_range(i));
        return array_[i];
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::reference VectorArray<T, N>::front()
{
        return *begin();
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::const_reference VectorArray<T, N>::front() const
{
        return *begin();
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::reference VectorArray<T, N>::back()
{
        return *(end() - 1);
}

template<typename T, std::size_t N>
typename VectorArray<T, N>::const_reference VectorArray<T, N>::back() const
{
        return *(end() - 1);
}

template<typename T, std::size_t N>
void VectorArray<T, N>::push_back(const_reference t)
{
        increment_size();
        back() = t;
}

template<typename T, std::size_t N>
void VectorArray<T, N>::pop_back()
{
        decrement_size();
}

template<typename T, std::size_t N>
void VectorArray<T, N>::increment_size()
{
        assert(!full());
        ++size_;
        assert(invariant());
}

template<typename T, std::size_t N>
void VectorArray<T, N>::decrement_size()
{
        assert(!empty());
        --size_;
        assert(invariant());
}

template<typename T, std::size_t N>
void VectorArray<T, N>::clear()
{
        size_ = 0;
        assert(invariant());
}

template<typename T, std::size_t N>
bool VectorArray<T, N>::invariant() const
{
        return size() <= max_size();
}

template<typename T, std::size_t N>
bool VectorArray<T, N>::within_range(size_type i) const
{
        return i < size();
}

template<typename T, std::size_t N>
bool VectorArray<T, N>::within_bounds(size_type i) const
{
        return i < max_size();
}

}       // namespace dctl
