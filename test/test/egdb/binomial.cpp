#include <cstddef>                              // ptrdiff_t
#include <limits>                               // max
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK_LE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/egdb/binomial.hpp>               // Binomial

namespace dctl {
namespace egdb {

BOOST_AUTO_TEST_SUITE(EndgameDatabaseBinomial)

BOOST_AUTO_TEST_CASE(CoefficientsAreSymmetric)
{
        auto constexpr N = 60;
        auto constexpr K = (N + 1) / 2;
        using binomial = Binomial<N, K>;

        for (auto n = 1; n <= N; ++n)
                for (auto k = 0; k <= (n + 1) / 2; ++k)
                        BOOST_CHECK_EQUAL(binomial::coefficient(n, k), binomial::coefficient(n, n - k));
}

BOOST_AUTO_TEST_CASE(CoefficientsSatisfyPascalTriangle)
{
        auto constexpr N = 60;
        auto constexpr K = (N + 1) / 2;
        using binomial = Binomial<N, K>;

        for (auto n = 1; n <= N; ++n)
                for (auto k = 0; k <= (n + 1) / 2; ++k)
                        BOOST_CHECK_EQUAL(binomial::coefficient(n, k), binomial::coefficient(n - 1, k - 1) + binomial::coefficient(n - 1, k));
}

BOOST_AUTO_TEST_CASE(CoefficientsSatisfyNewtonTheorem)
{
        auto constexpr N = 60;
        auto constexpr K = (N + 1) / 2;
        using binomial = Binomial<N, K>;

        for (auto n = 1; n <= N; ++n) {
                std::ptrdiff_t sum = 0;
                for (auto k = 0; k <= n; ++k)
                        sum += binomial::coefficient(n, k);
                BOOST_CHECK_EQUAL(sum, std::ptrdiff_t{1} << n);
        }
}

BOOST_AUTO_TEST_CASE(CoefficientsFitIntoPtrDiffT)
{
        using binomial = Binomial<384, 30>;
        auto const N = std::numeric_limits<std::ptrdiff_t>::max();

        BOOST_CHECK_LE(binomial::coefficient( 60, 30), N);
        BOOST_CHECK_LE(binomial::coefficient( 72, 20), N);
        BOOST_CHECK_LE(binomial::coefficient( 95, 16), N);
        BOOST_CHECK_LE(binomial::coefficient( 98, 15), N);
        BOOST_CHECK_LE(binomial::coefficient(128, 13), N);
        BOOST_CHECK_LE(binomial::coefficient(256, 10), N);
        BOOST_CHECK_LE(binomial::coefficient(384,  9), N);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace egdb
}       // namespace dctl
