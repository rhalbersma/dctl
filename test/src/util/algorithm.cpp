#include <dctl/util/algorithm.hpp>           // insertion_sort, selection_sort
#include <boost/algorithm/cxx11/iota.hpp>       // iota_n
#include <boost/range/algorithm.hpp>            // random_shuffle
#include <boost/range/algorithm_ext.hpp>        // is_sorted
#include <boost/range/irange.hpp>               // irange
#include <boost/range/iterator_range.hpp>       // copy_range
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <iterator>                             // back_inserter
#include <vector>                               // vector

BOOST_AUTO_TEST_SUITE(UtilityAlgorithm)

auto const N = 1ULL << 10;

BOOST_AUTO_TEST_CASE(IsSortedAfterInsertionSort)
{
        auto v = boost::copy_range<std::vector<int>>(boost::irange(0ULL, N));
        boost::random_shuffle(v);
        BOOST_CHECK(!boost::is_sorted(v));
        dctl::util::insertion_sort(v.begin(), v.end());
        BOOST_CHECK(boost::is_sorted(v));
}

BOOST_AUTO_TEST_CASE(IsSortedAfterSelectionSort)
{
        auto v = boost::copy_range<std::vector<int>>(boost::irange(0ULL, N));
        boost::random_shuffle(v);
        BOOST_CHECK(!boost::is_sorted(v));
        dctl::util::selection_sort(v.begin(), v.end());
        BOOST_CHECK(boost::is_sorted(v));
}

BOOST_AUTO_TEST_SUITE_END()
