#include <algorithm>                            // generate_n, is_sorted
#include <cstdlib>                              // rand, srand
#include <iterator>                             // back_inserter
#include <vector>                               // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <dctl/utility/algorithm.hpp>           // insertion_sort, selection_sort

namespace dctl {
namespace algorithm {

BOOST_AUTO_TEST_SUITE(TestAlgorithm)

BOOST_AUTO_TEST_CASE(InsertionSelectionSort)
{
        auto const N = 100;
        std::vector<int> v;
        v.reserve(N);

        std::srand(47110815);
        std::generate_n(std::back_inserter(v), N, [](){
                return std::rand();
        });
        auto w = v;

        insertion_sort(begin(v), end(v));
        selection_sort(begin(w), end(w));

        BOOST_CHECK(std::is_sorted(begin(v), end(v)));
        BOOST_CHECK(std::is_sorted(begin(w), end(w)));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace algorithm
}       // namespace dctl
