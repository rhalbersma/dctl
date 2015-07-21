#pragma once
#include <cassert>      // assert
#include <cstddef>      // size_t
#include <iterator>     // distance

namespace dctl {
namespace util {

template<class T, std::size_t N>
class bounded_vector
{
        T data_[N];
        std::size_t size_ = 0;
public:
        auto begin() const
        {
                return &data_[0];
        }

        auto begin()
        {
                return &data_[0];
        }

        auto end() const
        {
                return begin() + size();
        }

        auto end()
        {
                return begin() + size();
        }

        auto const& operator[](std::size_t n) const
        {
                return data_[n];
        }

        auto& operator[](std::size_t n)
        {
                return data_[n];
        }

        auto const& front() const
        {
                assert(!empty());
                return data_[0];
        }

        auto& front()
        {
                assert(!empty());
                return data_[0];
        }

        auto const& back() const
        {
                assert(!empty());
                return data_[size_ - 1];
        }

        auto& back()
        {
                assert(!empty());
                return data_[size_ - 1];
        }

        auto push_back(T const& val)
        {
                assert(size() < N);
                data_[size_++] = val;
        }

        template<class... Args>
        auto emplace_back(Args&&... args)
        {
                data_[size_++] = T(std::forward<Args>(args)...);
        }

        auto pop_back()
        {
                assert(!empty());
                --size_;
        }

        auto size() const
        {
                return size_;
        }

        auto empty() const
        {
                return !size();
        }

        template<class Iterator>
        auto erase(Iterator first, Iterator)
        {
                size_ = std::distance(begin(), first);
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
