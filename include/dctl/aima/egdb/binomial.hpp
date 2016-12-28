#pragma once
#include <array>        // array
#include <cassert>      // assert
#include <cstddef>      // size_t
#include <cstdint>      // int64_t

namespace dctl {
namespace egdb {
namespace detail {

// https://en.wikipedia.org/wiki/Binomial_coefficient
template<int N, int K>
class binomial
{
        using table_type = std::array<std::array<int64_t, K + 1>, N + 1>;
        static const table_type table;

        // https://en.wikipedia.org/wiki/Pascal's_triangle
        static auto pascal_triangle() noexcept
        {
                table_type choose;

                for (std::size_t n = 0; n < K; ++n) {
                        for (std::size_t k = n + 1; k < K + 1; ++k) {
                                choose[n][k] = 0LL;
                        }
                }
                choose[0][0] = 1LL;
                for (std::size_t n = 1; n < N + 1; ++n) {
                        choose[n][0] = 1LL;
                }
                for (std::size_t n = 1; n < K + 1; ++n) {
                        choose[n][n] = 1ULL;
                }
                for (std::size_t n = 1; n < K + 1; ++n) {
                        for (std::size_t k = 1; k < n; ++k) {
                                choose[n][k] = choose[n - 1][k - 1] + choose[n - 1][k];
                        }
                }
                for (std::size_t n = K + 1; n < N + 1; ++n) {
                        for (std::size_t k = 1; k < K + 1; ++k) {
                                choose[n][k] = choose[n - 1][k - 1] + choose[n - 1][k];
                        }
                }
                return choose;
        }
public:
        static auto coefficient(int n, int k) // Throws: Nothing.
                -> int64_t
        {
                if (k > n - k) {
                        k = n - k;
                }
                if (k < 0 || n < 0) {
                        return 0LL;
                }
                assert(0 <= n); assert(n <= N);
                assert(0 <= k); assert(k <= K);
                assert(k <= n / 2);
                return table[static_cast<std::size_t>(n)][static_cast<std::size_t>(k)];
        }
};

template<int N, int K>
const typename binomial<N, K>::table_type
binomial<N, K>::table = binomial<N, K>::pascal_triangle();

}       // namespace detail

template<int N = 64, int K = N / 2>
auto choose(int const n, int const k) // Throws: Nothing.
{
        // choose(n, k) shall not overflow using signed 64-integers.
        // For n <= 64: all k <= n/2 (e.g. Western chess board).
        // For n <= 90: all k <= 18  (e.g. Chinese chess board).
        static_assert(N <= 64 || (N <= 90 && K <= 18));

        return detail::binomial<N, K>::coefficient(n, k);
}

}       // namespace egdb
}       // namespace dctl
