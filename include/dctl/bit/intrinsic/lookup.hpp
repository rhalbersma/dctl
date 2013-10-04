#pragma once
#include <cassert>                      // assert
#include <limits>                       // digits
#include <type_traits>                  // integral_constant

namespace dctl {
namespace bit {
namespace lookup {

template<class U = unsigned char>
class table
{
public:
        template<class T>
        static constexpr auto ctz(T x) noexcept
        {
                auto n = 0;
                for (auto i = 0; i < num_blocks<T>::value; ++i) {
                        if (auto const b = block_mask(x, i)) {
                                n += ctz_[b];
                                break;
                        }
                        n += std::numeric_limits<U>::digits;
                }
                assert(post_condition<T>(n));
                return n;
        }

        template<class T>
        static constexpr auto clz(T x) noexcept
        {
                auto n = 0;
                for (auto i = num_blocks<T>::value - 1; i >= 0; --i) {
                        if (auto const b = block_mask(x, i)) {
                                n += clz_[b];
                                break;
                        }
                        n += std::numeric_limits<U>::digits;
                }
                assert(post_condition<T>(n));
                return n;
        }

        template<class T>
        static constexpr auto popcount(T x) noexcept
        {
                auto n = 0;
                for (auto i = 0; i < num_blocks<T>::value; ++i)
                        n += popcount_[block_mask(x, i)];
                assert(post_condition<T>(n));
                return n;
        }

private:
        // implementation

        template<class T>
        struct num_blocks
        :
                std::integral_constant<int, sizeof(T) / sizeof(U)>
        {};

        template<class T>
        static constexpr auto block_mask(T x, int i)
        {
                return static_cast<U>(x >> (i * std::numeric_limits<U>::digits));
        }

        template<class T>
        static constexpr auto post_condition(int n) noexcept
        {
                return 0 <= n && n <= std::numeric_limits<T>::digits;
        }

        // representation

        static constexpr int ctz_[] =
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

        static constexpr int clz_[] =
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

template<class U> constexpr int table<U>::ctz_[];
template<class U> constexpr int table<U>::clz_[];
template<class U> constexpr int table<U>::popcount_[];

using detail = table<>;

template<class T>
constexpr auto ctz(T x) noexcept
{
        return detail::ctz(x);
}

template<class T>
constexpr auto clz(T x) noexcept
{
        return detail::clz(x);
}

template<class T>
constexpr auto popcount(T x) noexcept
{
        return detail::popcount(x);
}

}       // namespace lookup
}       // namespace bit
}       // namespace dctl
