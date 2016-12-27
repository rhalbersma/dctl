#pragma once
#include <dctl/utility/fill_array.hpp>                  // fill_array
#include <boost/math/special_functions/binomial.hpp>    // binomial_coefficient
#include <algorithm>                                    // min
#include <array>                                        // array
#include <cassert>                                      // assert
#include <cstddef>                                      // size_t
#include <cstdint>                                      // int64_t

namespace dctl {
namespace egdb {

template<int N, int K>
class binomial
{
        using table_type = std::array<std::array<int64_t, K+1>, N+1>;
        static const table_type table;
public:
        static int64_t coefficient(int n, int k)
        {
                k = std::min(k, n - k);
                if (k < 0 || n < 0) {
                        return 0;
                }
                assert(0 <= n); assert(n <= N);
                assert(0 <= k); assert(k <= K);
                assert(k <= n / 2);
                return table[static_cast<std::size_t>(n)][static_cast<std::size_t>(k)];
        }
};

template<int N, int K>
const typename binomial<N, K>::table_type
binomial<N, K>::table = fill_array<N+1>([](auto n){
        return fill_array<K+1>([&](auto k) {
                return k > n ? 0 : static_cast<int64_t>(boost::math::binomial_coefficient<double>(
                        static_cast<unsigned>(n), static_cast<unsigned>(k)
                ));
        });
});

inline
auto choose(int n, int k)
{
        return binomial<50, 12>::coefficient(n, k);
}

}       // namespace egdb
}       // namespace dctl
