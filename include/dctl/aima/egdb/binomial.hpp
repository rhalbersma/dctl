#pragma once
#include <array>        // array
#include <cassert>      // assert
#include <cstddef>      // size_t
#include <cstdint>      // int64_t

namespace dctl {
namespace egdb {

template<int N = 64, int K = N / 2>
class binomial
{
        using table_type = std::array<std::array<int64_t, K + 1>, N + 1>;
        static const table_type table;

        static auto fill_table() noexcept
        {
                table_type triangle;

                for (std::size_t n = 0; n < K; ++n) {
                        for (std::size_t k = n + 1; k < K + 1; ++k) {
                                triangle[n][k] = 0LL;
                        }
                }
                triangle[0][0] = 1LL;
                for (std::size_t n = 1; n < N + 1; ++n) {
                        triangle[n][0] = 1LL;
                }
                for (std::size_t n = 1; n < K + 1; ++n) {
                        triangle[n][n] = 1ULL;
                }
                for (std::size_t n = 1; n < K + 1; ++n) {
                        for (std::size_t k = 1; k < n; ++k) {
                                triangle[n][k] = triangle[n - 1][k - 1] + triangle[n - 1][k];
                        }
                }
                for (std::size_t n = K + 1; n < N + 1; ++n) {
                        for (std::size_t k = 1; k < K + 1; ++k) {
                                triangle[n][k] = triangle[n - 1][k - 1] + triangle[n - 1][k];
                        }
                }
                return triangle;
        }
public:
        static auto coefficient(int n, int k)
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
binomial<N, K>::table = binomial<N, K>::fill_table();

inline
auto choose(int n, int k)
{
        return binomial<>::coefficient(n, k);
}

}       // namespace egdb
}       // namespace dctl
