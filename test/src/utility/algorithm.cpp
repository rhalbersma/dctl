#include <dctl/utility/algorithm.hpp>   // insertion_sort, selection_sort
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>                    // generate_n, is_sorted
#include <cstdlib>                      // rand, srand
#include <iterator>                     // back_inserter
#include <vector>                       // vector

BOOST_AUTO_TEST_SUITE(UtilityAlgorithm)

BOOST_AUTO_TEST_CASE(IsSortedAfterInsertionSort)
{
        auto const N = 1 << 10;
        std::vector<int> v;
        v.reserve(N);
        std::srand(47110815);
        std::generate_n(std::back_inserter(v), N, [](){ return std::rand(); });

        dctl::util::insertion_sort(begin(v), end(v));
        BOOST_CHECK(std::is_sorted(begin(v), end(v)));
}

BOOST_AUTO_TEST_CASE(IsSortedAfterSelectionSort)
{
        auto const N = 1 << 10;
        std::vector<int> v;
        v.reserve(N);
        std::srand(47110815);
        std::generate_n(std::back_inserter(v), N, [](){ return std::rand(); });

        dctl::util::selection_sort(begin(v), end(v));
        BOOST_CHECK(std::is_sorted(begin(v), end(v)));
}

BOOST_AUTO_TEST_SUITE_END()
