#pragma once
#include <cstddef>                      // std::size_t
#include <cstdint>                      // uint8_t
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <dctl/bit/bit_fwd.hpp>         // is_single
#include <dctl/utility/int.hpp>         // num_bits

namespace dctl {
namespace bit {
namespace lookup {

template<typename Block>
class table
{
public:
        template<typename T>
        static std::size_t count(T t)
        {
                int const num_blocks = sizeof(T) / sizeof(Block);
                auto n = 0;
                for (auto i = 0; i < num_blocks; ++i)
                        n += count_[block(t, i)];
                return n;
        }

        template<typename T>
        static int index(T t)
        {
                BOOST_ASSERT(is_single(t));
                int const num_blocks = sizeof(T) / sizeof(Block);
                for (auto i = 0; i < num_blocks; ++i)
                        if (auto const b = block(t, i))
                                return offset<T>(i) + index_[b];
                BOOST_ASSERT(false);
                return 0;
        }

private:
        // implementation

        template<typename T>
        static Block block(T t, int i)
        {
                return static_cast<Block>(t >> (i * bits_per_block));
        }

        template<typename T>
        static int offset(int i)
        {
                return i * (sizeof(T) / sizeof(Block));
        }

        // representation

        BOOST_STATIC_CONSTANT(int, bits_per_block = num_bits<Block>::value);
        static std::size_t const count_[];
        static int const index_[];
};

template<typename Block>
std::size_t const table<Block>::count_[] = {
         0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
         1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
         1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
         1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
         2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
         3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
         3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
         4,  5,  5,  6,  5,  6,  6,  7,  5,  6,  6,  7,  6,  7,  7,  8
};

template<typename Block>
int const table<Block>::index_[] = {
         0,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         6,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         7,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         6,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         5,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
         4,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0
};

typedef table<uint8_t> detail;

template<typename T>
int count(T b)
{
        return detail::count(b);
}

template<typename T>
int index(T b)
{
        return detail::index(b);
}

}       // namespace lookup
}       // namespace bit
}       // namespace dctl
