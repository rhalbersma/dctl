#include <dctl/utility/algorithm.hpp>   // insertion_sort, selection_sort
#include <range/v3/all.hpp>             // is_sorted
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <vector>                       // vector

BOOST_AUTO_TEST_SUITE(UtilityAlgorithm)

using namespace ranges;

BOOST_AUTO_TEST_CASE(IsSortedAfterInsertionSort)
{
        auto const N = 1ULL << 10;
        std::vector<int> v;
        v.reserve(N);
        v |= action::push_back(view::iota(0, N));
        random_shuffle(v);

        dctl::util::insertion_sort(begin(v), end(v));
        BOOST_CHECK(is_sorted(v));
}

BOOST_AUTO_TEST_CASE(IsSortedAfterSelectionSort)
{
        auto const N = 1ULL << 10;
        std::vector<int> v;
        v.reserve(N);
        v |= action::push_back(view::iota(0, N));
        random_shuffle(v);

        dctl::util::selection_sort(begin(v), end(v));
        BOOST_CHECK(is_sorted(v));
}

BOOST_AUTO_TEST_SUITE_END()
