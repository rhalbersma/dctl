#pragma once
#include <cassert>                      // assert
#include <limits>                       // digits
#include <stdexcept>                    // invalid_argument
#include <type_traits>                  // integral_constant

namespace dctl {
namespace bit {
namespace lookup {

template<class U = unsigned char>
class table
{
public:
        template<class T>
        static constexpr auto ctznz(T x)
        {
                for (auto i = 0, n = 0; i < num_blocks<T>::value; ++i) {
                        auto const b = block_mask(x, i);
                        n += ctznz_[b];
                        if (b) return n;
                }
                throw std::invalid_argument("ctznz requires non-zero argument");
        }

        template<class T>
        static constexpr auto clznz(T x)
        {
                for (auto i = num_blocks<T>::value - 1, n = 0; i >= 0; --i) {
                        auto const b = block_mask(x, i);
                        n += clznz_[b];
                        if (b) return n;
                }
                throw std::invalid_argument("clznz requires non-zero argument");
        }

        template<class T>
        static constexpr auto popcount(T x) noexcept
        {
                auto n = 0;
                for (auto i = 0; i < num_blocks<T>::value; ++i)
                        n += popcount_[block_mask(x, i)];
                assert(0 <= n && n <= std::numeric_limits<T>::digits);
                return n;
        }

private:
        // implementation

        template<class T>
        using num_blocks = std::integral_constant<int, sizeof(T) / sizeof(U)>;

        template<class T>
        static constexpr auto block_mask(T x, int i)
        {
                return static_cast<U>(x >> (i * std::numeric_limits<U>::digits));
        }

        // representation

        static constexpr int ctznz_[] =
        {
                8,  0,  1,  0,  2,  0,  1,  0,  3,  0,  1,  0,  2,  0,  1,  0,
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

        static constexpr int clznz_[] =
        {
                8,  7,  6,  6,  5,  5,  5,  5,  4,  4,  4,  4,  4,  4,  4,  4,
                3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
                2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
                2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
                1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };

        static constexpr int popcount_[]  =
        {
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

};

template<class U> constexpr int table<U>::ctznz_[];
template<class U> constexpr int table<U>::clznz_[];
template<class U> constexpr int table<U>::popcount_[];

using detail = table<>;

template<class T>
constexpr auto ctznz(T x)
{
        assert(x != 0);
        return detail::ctznz(x);
}

template<class T>
constexpr auto clznz(T x)
{
        assert(x != 0);
        return detail::clznz(x);
}

template<class T>
constexpr auto popcount(T x) noexcept
{
        return detail::popcount(x);
}

}       // namespace lookup
}       // namespace bit
}       // namespace dctl
