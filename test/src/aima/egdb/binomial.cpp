#include <dctl/aima/egdb/binomial.hpp>    // Binomial
#include <boost/range/irange.hpp>       // irange
#include <boost/range/numeric.hpp>      // accumulate
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_CHECK_LE, BOOST_AUTO_TEST_SUITE_END
#include <cstddef>                      // ptrdiff_t
#include <limits>                       // max

namespace dctl {
namespace egdb {

BOOST_AUTO_TEST_SUITE(EndgameDatabaseBinomial)

BOOST_AUTO_TEST_CASE(CoefficientsAreSymmetric)
{
        auto constexpr N = 60;
        auto constexpr K = (N + 1) / 2;
        using Binomial = BinomialTable<N, K>;

        for (auto n = 1; n <= N; ++n)
                for (auto k = 0; k <= (n + 1) / 2; ++k)
                        BOOST_CHECK_EQUAL(Binomial::coefficient(n, k), Binomial::coefficient(n, n - k));
}

BOOST_AUTO_TEST_CASE(CoefficientsSatisfyPascalTriAngle)
{
        auto constexpr N = 60;
        auto constexpr K = (N + 1) / 2;
        using Binomial = BinomialTable<N, K>;

        for (auto n = 1; n <= N; ++n)
                for (auto k = 0; k <= (n + 1) / 2; ++k)
                        BOOST_CHECK_EQUAL(Binomial::coefficient(n, k), Binomial::coefficient(n - 1, k - 1) + Binomial::coefficient(n - 1, k));
}

BOOST_AUTO_TEST_CASE(CoefficientsSatisfyNewtonTheorem)
{
        auto constexpr N = 60;
        auto constexpr K = (N + 1) / 2;
        using Binomial = BinomialTable<N, K>;

        for (auto n = 1; n <= N; ++n) {
                auto const lhs = boost::accumulate(
                        boost::irange(0, n + 1),
                        std::ptrdiff_t{0},
                        [=](auto sum, auto k) {
                        return sum + Binomial::coefficient(n, k);
                });
                auto const rhs = std::ptrdiff_t{1} << n;
                BOOST_CHECK_EQUAL(lhs, rhs);
        }
}

BOOST_AUTO_TEST_CASE(CoefficientsFitIntoPtrDiffT)
{
        using Binomial = BinomialTable<384, 30>;
        auto const N = std::numeric_limits<std::ptrdiff_t>::max();

        BOOST_CHECK_LE(Binomial::coefficient( 60, 30), N);
        BOOST_CHECK_LE(Binomial::coefficient( 72, 20), N);
        BOOST_CHECK_LE(Binomial::coefficient( 95, 16), N);
        BOOST_CHECK_LE(Binomial::coefficient( 98, 15), N);
        BOOST_CHECK_LE(Binomial::coefficient(128, 13), N);
        BOOST_CHECK_LE(Binomial::coefficient(256, 10), N);
        BOOST_CHECK_LE(Binomial::coefficient(384,  9), N);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace egdb
}       // namespace dctl
