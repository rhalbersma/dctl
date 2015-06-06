#include <dctl/utility/algorithm.hpp>   // insertion_sort, selection_sort
#include <range/v3/all.hpp>             // is_sorted, view::iota, action::shuffle
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <random>                       // mt19937
#include <vector>                       // vector

BOOST_AUTO_TEST_SUITE(UtilityAlgorithm)

using namespace ranges;

BOOST_AUTO_TEST_CASE(IsSortedAfterInsertionSort)
{
        std::mt19937 gen;
        auto const N = 1ULL << 10;
        std::vector<int> v;
        v |= action::push_back(view::iota(0, N)) | action::shuffle(gen);

        dctl::util::insertion_sort(begin(v), end(v));
        BOOST_CHECK(is_sorted(v));
}

BOOST_AUTO_TEST_CASE(IsSortedAfterSelectionSort)
{
        std::mt19937 gen;
        auto const N = 1ULL << 10;
        std::vector<int> v;
        v |= action::push_back(view::iota(0, N)) | action::shuffle(gen);

        dctl::util::selection_sort(begin(v), end(v));
        BOOST_CHECK(is_sorted(v));
}

BOOST_AUTO_TEST_SUITE_END()
