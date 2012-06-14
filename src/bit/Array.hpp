#pragma once
#include <algorithm>                    // fill_n
#include <array>                        // array
#include <cstddef>                      // size_t
#include <cstdint>                      // uint64_t
#include <boost/operators.hpp>          // bitwise, shiftable, totally_ordered

namespace dctl {
namespace bit {

template
<
        std::size_t N, 
        typename T = uint64_t 
>
class Array
:       boost::bitwise< Array<N, T> >
,       boost::shiftable< Array<N, T>, int >
,       boost::totally_ordered< Array<N, T> >
{
public:
        // structors 

        Array()
        {
                // no-op
        }

        explicit Array(T const& value)
        {
                bits_[0] = value;
                std::fill_n(std::begin(bits_)+1, N - 1, T(0));
        }

        void fill(T const& value)
        {
                bits_.fill(value);
        }

        // modifiers

        void swap(Array& other)
        {
                bits_.swap(other.bits_);
        }

        Array& flip()
        {
                for (auto i = 0; i < N; ++i)
                        bits_[i] = ~bits_[i];
                return *this;
        }

#define DCTL_PP_BITWISE_ASSIGN(OP)                              \
        Array& operator OP ## = (Array const& other)            \
        {                                                       \
                for (auto i = 0; i < N; ++i)                    \
                        bits_[i] OP ## = other.bits_[i];        \
                return *this;                                   \
        }

        DCTL_PP_BITWISE_ASSIGN(&)       // operator&=
        DCTL_PP_BITWISE_ASSIGN(^)       // operator^=
        DCTL_PP_BITWISE_ASSIGN(|)       // operator|=

#undef DCTL_PP_OP_ASSIGN

        Array& operator<<=(int pos)
        {
                BOOST_ASSERT(0 <= pos && pos < NUM_BITS);

                if (!pos) return *this;

                std::size_t const between = pos / BITS_PER_BLOCK;
                std::size_t const within  = pos % BITS_PER_BLOCK;

                if (!within) {                      
                        for (auto i = N - 1; i >= between; --i)
                                bits_[i] = bits_[i - between];
                } else {
                        std::size_t const co_within = BITS_PER_BLOCK - within;

                        for (auto i = N - 1; i > between; --i)
                                bits_[i] = 
                                        (bits_[i - between    ] <<    within) |
                                        (bits_[i - between - 1] >> co_within)
                                ;
                        bits_[between] = bits_[0] << within;
                }
                std::fill_n(std::begin(bits_), between, T(0));

                return *this;
        }

        Array& operator>>=(int pos)
        {
                BOOST_ASSERT(0 <= pos && pos < NUM_BITS);

                if (!pos) return *this;

                std::size_t const between = pos / BITS_PER_BLOCK;
                std::size_t const within  = pos % BITS_PER_BLOCK;
                std::size_t const co_between = N - between - 1;

                if (!within) {
                        for (auto i = 0; i <= co_between; ++i)
                                bits_[i] = bits_[i + between];
                } else {
                        std::size_t const co_within = BITS_PER_BLOCK - within;

                        for (auto i = 0; i < co_between; ++i)
                                bits_[i] = 
                                        (bits_[i + between    ] >>    within) |
                                        (bits_[i + between + 1] << co_within)
                                ;
                        bits_[co_between] = bits_[N - 1] >> within;
                }
                std::fill_n(bits_[co_between + 1], between, T(0));

                return *this;
        }

#define DCTL_PP_RELATIONAL(OP)                          \
        bool operator OP (Array const& other) const     \
        {                                               \
                return bits_ OP other.bits_;            \
        }

        DCTL_PP_RELATIONAL(==)  // operator==
        DCTL_PP_RELATIONAL(<)   // operator<

#undef DCTL_PP_OP_RELATION

        // queries

        bool empty() const
        {
                return bits_.empty();
        }

private:
        static std::size_t const BITS_PER_BLOCK = 8 * sizeof(T);
        static std::size_t const NUM_BITS = N * BITS_PER_BLOCK;

        std::array<T, N> bits_;
};

template<std::size_t N, typename T>
Array<N, T> operator~(const Array<N, T>& lhs)
{
        return Array<N, T>(lhs).flip();
}

}       // namespace bit
}       // namespace dctl
