#include <dctl/utility/algorithm.hpp>   // insertion_sort, selection_sort
#include <boost/algorithm/cxx11/iota.hpp>
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>
#include <iterator>
#include <random>                       // mt19937
#include <vector>                       // vector

BOOST_AUTO_TEST_SUITE(UtilityAlgorithm)

BOOST_AUTO_TEST_CASE(IsSortedAfterInsertionSort)
{
        std::mt19937 gen;
        auto const N = 1ULL << 10;
        std::vector<int> v;
        boost::algorithm::iota_n(std::back_inserter(v), 0, N);
        std::shuffle(v.begin(), v.end(), gen);

        dctl::util::insertion_sort(v.begin(), v.end());
        BOOST_CHECK(std::is_sorted(v.cbegin(), v.cend()));
}

BOOST_AUTO_TEST_CASE(IsSortedAfterSelectionSort)
{
        std::mt19937 gen;
        auto const N = 1ULL << 10;
        std::vector<int> v;
        boost::algorithm::iota_n(std::back_inserter(v), 0, N);
        std::shuffle(v.begin(), v.end(), gen);

        dctl::util::selection_sort(v.begin(), v.end());
        BOOST_CHECK(std::is_sorted(v.cbegin(), v.cend()));
}

BOOST_AUTO_TEST_SUITE_END()
