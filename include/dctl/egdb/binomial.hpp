#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <array>        // array
#include <cassert>      // assert
#include <cstddef>      // size_t
#include <cstdint>      // int64_t
#include <type_traits>  // is_integral

namespace dctl::egdb {
namespace detail {

// https://en.wikipedia.org/wiki/Binomial_coefficient
template<int N, int K, class IntegralType>
class binomial
{
        constexpr static auto static_assert_type_traits() noexcept
        {
                static_assert(0 <= K); static_assert(K <= N);
                static_assert(std::is_integral_v<IntegralType>);
        }

        // https://en.wikipedia.org/wiki/Pascal's_triangle
        constexpr static auto pascal_triangle = []() {
                auto choose = std::array<std::array<IntegralType, K + 1>, N + 1>{};

                /*

                               0 1 2 3 . K . . . . . N
                             X-----------+-----------+
                          0 /1\0 0 0 . 0/           / 0
                         1 /1 1\0 0 . 0/           / 1
                        2 /1 2 1\0 . 0/           / 2
                       3 /1 3 3 1\. 0/           / 3
                      . /. . . . .\0/           / .
                     K /1 K . . K 1X           / K
                    . /-----------/ \         / .
                   . /.          /   \       / .
                  . /.          /     \     / .
                 . /.          /       \   / .
                N /1 N . . . ./. . . N 1\ / N
                 +-----------+-----------X
                  0 1 2 3 . K . . . . . N

                */

                // upper-right triangle: width K, height K
                for (std::size_t n = 0; n < K; ++n) {
                        for (std::size_t k = n + 1; k < K + 1; ++k) {
                                choose[n][k] = 0;
                        }
                }

                // upper-left triangle: width K+1, height K+1
                choose[0][0] = 1;
                for (std::size_t n = 1; n < K + 1; ++n) {
                        choose[n][0] = 1;
                        for (std::size_t k = 1; k < n; ++k) {
                                choose[n][k] = choose[n - 1][k - 1] + choose[n - 1][k];
                        }
                        choose[n][n] = 1;
                }

                // lower rectangle: width K+1, height N-K
                for (std::size_t n = K + 1; n < N + 1; ++n) {
                        choose[n][0] = 1;
                        for (std::size_t k = 1; k < K + 1; ++k) {
                                choose[n][k] = choose[n - 1][k - 1] + choose[n - 1][k];
                        }
                }

                return choose;
        }();

public:
        constexpr static auto coefficient(int n, int k) // Throws: Nothing.
                -> IntegralType
        {
                if (k > n - k) {
                        k = n - k;
                }
                if (k < 0 || n < 0) {
                        return 0;
                }
                assert(0 <= n); assert(n <= N);
                assert(0 <= k); assert(k <= K);
                assert(k <= n / 2);
                return pascal_triangle[static_cast<std::size_t>(n)][static_cast<std::size_t>(k)];
        }
};

}       // namespace detail

template<int N = 64, int K = N / 2>
constexpr auto choose(int const n, int const k) // Throws: Nothing.
{
        // choose(n, k) shall not overflow using signed 64-integers.
        // For n <= 64: all k <= n/2 (e.g. Western chess board).
        // For n <= 90: all k <= 18  (e.g. Chinese chess board).
        static_assert((N <= 64 && K <= N / 2) || (N <= 90 && K <= 18));
        assert(0 <= n); assert(n <= N);

        return detail::binomial<N, K, int64_t>::coefficient(n, k);
}

}       // namespace dctl::egdb
