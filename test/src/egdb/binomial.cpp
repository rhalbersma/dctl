//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/egdb/binomial.hpp>       // choose
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END

BOOST_AUTO_TEST_SUITE(Binomial)

using dctl::egdb::choose;
constexpr auto N = 64;

BOOST_AUTO_TEST_CASE(CoefficientsAreSymmetric)
{
        for (auto n = 0; n < N + 1; ++n) {
                for (auto k = 0; k < n + 1; ++k) {
                        BOOST_CHECK_EQUAL(choose(n, k), choose(n, n - k));
                }
        }
}

BOOST_AUTO_TEST_CASE(CoefficientsSatisfyPascalTriangle)
{
        for (auto n = 0; n < N + 1; ++n) {
                BOOST_CHECK_EQUAL(choose(n, 0), 1);
                for (auto k = 1; k < n; ++k) {
                        BOOST_CHECK_EQUAL(choose(n, k), choose(n - 1, k - 1) + choose(n - 1, k));
                }
                BOOST_CHECK_EQUAL(choose(n, n), 1);
        }
}

BOOST_AUTO_TEST_CASE(CoefficientsSatisfyNewtonTheorem)
{
        // Type int64_t values lie within the half-open interval [-2^63, 2^63).
        // Hence, we can test Newton's Theorem for at most n = 62.
        for (auto n = 0; n < N - 1; ++n) {
                auto sum = 0LL;
                for (auto k = 0; k < n + 1; ++k) {
                        sum += choose(n, k);
                }
                BOOST_CHECK_EQUAL(sum, 1LL << n);
        }
}

BOOST_AUTO_TEST_CASE(SmallCoefficientsAreTrivial)
{
        for (auto n = 0; n <= N; ++n) {
                BOOST_CHECK_EQUAL(choose(n, -1    ), 0);
                BOOST_CHECK_EQUAL(choose(n,  0    ), 1);
                BOOST_CHECK_EQUAL(choose(n,  1    ), n);
                BOOST_CHECK_EQUAL(choose(n,  n - 1), n);
                BOOST_CHECK_EQUAL(choose(n,  n    ), 1);
                BOOST_CHECK_EQUAL(choose(n,  n + 1), 0);
        }
}

BOOST_AUTO_TEST_CASE(LargeCoefficientsMatchWolframAlpha)
{
        // https://www.wolframalpha.com/input/?i=binomial%5B64,+32%5D
        BOOST_CHECK_EQUAL(choose(64, 32), 1'832'624'140'942'590'534LL);

        // https://www.wolframalpha.com/input/?i=binomial%5B90,+18%5D
        BOOST_CHECK_EQUAL((choose<90, 18>)(90, 18), 3'789'648'142'708'598'775LL);
}

BOOST_AUTO_TEST_SUITE_END()
