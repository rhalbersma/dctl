#pragma once
#include <cassert>      // assert
#include <cstddef>      // size_t
#include <utility>      // move

namespace dctl {
namespace util {

template<class T, std::size_t N = 64>
class bounded_vector
{
        T data_[N];
        std::size_t size_ = 0;
public:
        auto begin() noexcept
        {
                return data_;
        }

        auto begin() const noexcept
        {
                return data_;
        }

        auto end() noexcept
        {
                return begin() + size();
        }

        auto end() const noexcept
        {
                return begin() + size();
        }

        auto empty() const
        {
                return !size();
        }

        auto size() const
        {
                return size_;
        }

        auto& operator[](std::size_t n)
        {
                assert(n < size());
                return data_[n];
        }

        auto const& operator[](std::size_t n) const
        {
                assert(n < size());
                return data_[n];
        }

        auto& front()
        {
                assert(!empty());
                return data_[0];
        }

        auto const& front() const
        {
                assert(!empty());
                return data_[0];
        }

        auto& back()
        {
                assert(!empty());
                return data_[size_ - 1];
        }

        auto const& back() const
        {
                assert(!empty());
                return data_[size_ - 1];
        }

        template<class... Args>
        auto emplace_back(Args&&... args)
        {
                assert(size() < N);
                data_[size_++] = T(std::forward<Args>(args)...);
        }

        auto push_back(T const& val)
        {
                assert(size() < N);
                data_[size_++] = val;
        }

        auto push_back(T&& val)
        {
                assert(size() < N);
                data_[size_++] = std::move(val);
        }

        auto pop_back()
        {
                assert(!empty());
                --size_;
        }

        auto clear() noexcept
        {
                size_ = 0;
        }
};

template<class T, std::size_t N>
auto begin(bounded_vector<T, N>& v)
{
        return v.begin();
}

template<class T, std::size_t N>
auto begin(bounded_vector<T, N> const& v)
{
        return v.begin();
}

template<class T, std::size_t N>
auto end(bounded_vector<T, N>& v)
{
        return v.end();
}

template<class T, std::size_t N>
auto end(bounded_vector<T, N> const& v)
{
        return v.end();
}

}       // namespace util
}       // namespace dctl
