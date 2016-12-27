#include <dctl/aima/egdb/binomial.hpp>  // binomial
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END

BOOST_AUTO_TEST_SUITE(Binomial)

constexpr auto N = 50;
constexpr auto K = N / 2;
using binomial = dctl::egdb::binomial<N, K>;

BOOST_AUTO_TEST_CASE(SmallCoefficientsAreTrivial)
{
        for (auto n = 0; n <= N; ++n) {
                BOOST_CHECK_EQUAL(binomial::coefficient(n, -1    ), 0);
                BOOST_CHECK_EQUAL(binomial::coefficient(n,  0    ), 1);
                BOOST_CHECK_EQUAL(binomial::coefficient(n,  1    ), n);
                BOOST_CHECK_EQUAL(binomial::coefficient(n,  n - 1), n);
                BOOST_CHECK_EQUAL(binomial::coefficient(n,  n    ), 1);
                BOOST_CHECK_EQUAL(binomial::coefficient(n,  n + 1), 0);
        }
}

BOOST_AUTO_TEST_CASE(CoefficientsAreSymmetric)
{
        for (auto n = 0; n <= N; ++n) {
                for (auto k = 0; k <= n / 2; ++k) {
                        BOOST_CHECK_EQUAL(binomial::coefficient(n, k), binomial::coefficient(n, n - k));
                }
        }
}

BOOST_AUTO_TEST_CASE(CoefficientsSatisfyPascalTriangle)
{
        for (auto n = 2; n <= N; ++n) {
                for (auto k = 1; k <= n / 2; ++k) {
                        BOOST_CHECK_EQUAL(binomial::coefficient(n, k), binomial::coefficient(n - 1, k - 1) + binomial::coefficient(n - 1, k));
                }
        }
}

BOOST_AUTO_TEST_CASE(CoefficientsSatisfyNewtonTheorem)
{
        for (auto n = 0; n <= N; ++n) {
                auto sum = 0LL;
                for (auto k = 0; k <=n; ++k) {
                        sum += binomial::coefficient(n, k);
                }
                BOOST_CHECK_EQUAL(sum, 1LL << n);
        }
}

BOOST_AUTO_TEST_CASE(LargeCoefficientsMatchWolframAlpha)
{
        namespace lib = dctl::egdb;
        using b050 = lib::binomial< 50, 25>; BOOST_CHECK_EQUAL(b050::coefficient( 50, 25), 126'410'606'437'752LL);
        using b064 = lib::binomial< 64, 14>; BOOST_CHECK_EQUAL(b064::coefficient( 64, 14),  47'855'699'958'816LL);
        using b072 = lib::binomial< 72, 13>; BOOST_CHECK_EQUAL(b072::coefficient( 72, 13),  70'907'466'006'720LL);
        using b081 = lib::binomial< 81, 12>; BOOST_CHECK_EQUAL(b081::coefficient( 81, 12),  70'724'320'184'700LL);
        using b090 = lib::binomial< 90, 11>; BOOST_CHECK_EQUAL(b090::coefficient( 90, 11),  41'604'694'413'840LL);
}

BOOST_AUTO_TEST_SUITE_END()
