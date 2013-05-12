#pragma once
#include <iterator>                     // iterator

namespace dctl {
namespace packed {

template<typename> class bit_reference;
template<typename> class bit_iterator;

template<typename T>
class bit_reference
{
public:
        bit_reference(T m)
        :
                mask_(m)
        {}

        // reference cannot be rebound
        bit_reference& operator=(bit_reference const&) = delete;

        operator int() const
        {
                return static_cast<int>(__builtin_ctzll(mask_));
        }

        bit_iterator<T> operator&() const
        {
                return bit_iterator<T>(mask_);
        }

private:
        T mask_;
};

template<typename T>
class bit_iterator
:
        public std::iterator
        <
                std::forward_iterator_tag,
                int,
                int,
                bit_iterator<T>,
                bit_reference<T>
        >
{
public:
        bit_iterator(T m)
        :
                mask_(m)
        {}

        bit_reference<T> operator*() const
        {
                return bit_reference<T>(mask_);
        }

        bit_iterator& operator++()
        {
                mask_ &= mask_ - 1;
                return *this;
        }

        bit_iterator operator++(int)
        {
                auto old = *this;
                ++*this;
                return old;
        }

        friend bool operator==(bit_iterator const& lhs, bit_iterator const& rhs)
        {
                return lhs.mask_ == rhs.mask_;
        }

        friend bool operator!=(bit_iterator const& lhs, bit_iterator const& rhs)
        {
                return !(lhs == rhs);
        }

private:
        T mask_;
};

}       // namespace packed
}       // namespace dctl
