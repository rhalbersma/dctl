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
class Set
:       boost::bitwise< set<N< T> >
,       boost::shiftable< Set<N, T> >
,       boost::totally_ordered< Set<N, T> >
        > > >
{
public:
        Set& flip()
        {
                for (auto i = 0; i < N; ++i)
                        block_[i] = ~block_[i];
                return *this;
        }

#define DCTL_PP_BITWISE_ASSIGN(OP)                              \
        Set& operator OP ## = (Set const& other)                \
        {                                                       \
                for (auto i = 0; i < N; ++i)                    \
                        block_[i] OP ## = other.block_[i];      \
                return *this;                                   \
        }

        DCTL_PP_BITWISE_ASSIGN(&)       // operator&=
        DCTL_PP_BITWISE_ASSIGN(^)       // operator^=
        DCTL_PP_BITWISE_ASSIGN(|)       // operator|=

#undef DCTL_PP_OP_ASSIGN

        Set& operator<<=(int pos)
        {
                BOOST_ASSERT(0 <= pos < NUM_BITS);

                if (!pos) return *this;

                std::size_t const between = pos / BITS_PER_BLOCK;
                std::size_t const within  = pos % BITS_PER_BLOCK;

                if (!within) {                      
                        for (auto i = N - 1; i >= between; --i)
                                block_[i] = block_[i - between];
                } else {
                        std::size_t const co_within = BITS_PER_BLOCK - within;

                        for (auto i = N - 1; i > between; --i)
                                block_[i] = 
                                        (block_[i - between    ] <<    within) |
                                        (block_[i - between - 1] >> co_within)
                                ;
                        if (between < N)
                                block_[between] = block_[0] << within;
                }
                std::fill_n(std::begin(block_), between, T(0));

                return *this;
        }

        Set& operator>>=(int pos)
        {
                BOOST_ASSERT(0 <= pos < NUM_BITS);

                if (!pos) return *this;

                std::size_t const between = pos / BITS_PER_BLOCK;
                std::size_t const within  = pos % BITS_PER_BLOCK;
                std::size_t const co_between = N - between - 1;

                if (!within) {
                        for (auto i = 0; i <= co_between; ++i)
                                block_[i] = block_[i + between];
                } else {
                        std::size_t const co_within = BITS_PER_BLOCK - within;

                        for (auto i = 0; i < co_between; ++i)
                                block_[i] = 
                                        (block_[i + between    ] >>    within) |
                                        (block_[i + between + 1] << co_within)
                                ;
                        block_[co_between] = block_[N - 1] >> within;
                }
                std::fill_n(block_[co_between + 1], between, T(0));

                return *this;
        }

#define DCTL_PP_RELATIONAL(OP)                          \
        bool operator OP (Set const& other) const       \
        {                                               \
                return block_ OP other.block_;          \
        }

        DCTL_PP_RELATIONAL(==)  // operator==
        DCTL_PP_RELATIONAL(<)   // operator<

#undef DCTL_PP_OP_RELATION

        size_t count() const
        {
                return 0;
        }

        size_t find_first_one() const
        {
                return 0;
        }

private:
        static std::size_t const BITS_PER_BLOCK = 8 * sizeof(T);
        static std::size_t const NUM_BITS = N * BITS_PER_BLOCK;
        std::array<T, N> block_;
};

template<std::size_t N, typename T>
Set<N, T> operator~(const Set<N, T>& lhs)
{
        return Set<N, T>(lhs).flip();
}

}       // namespace bit
}       // namespace dctl
