#include <algorithm>                            // generate_n
#include <cstdlib>                              // rand, srand
#include <iterator>                             // back_inserter, begin, end
#include <vector>                               // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <boost/algorithm/cxx11/is_sorted.hpp>  // is_sorted
#include <dctl/utility/algorithm.hpp>           // insertion_sort, selection_sort

namespace dctl {
namespace algorithm {

BOOST_AUTO_TEST_SUITE(TestSTDAlgorithm)

BOOST_AUTO_TEST_CASE(testInsertionSelectionSort)
{
        auto const N = 100;
        std::vector<int> v;
        v.reserve(N);
        
        std::srand(47110815);
        std::generate_n(std::back_inserter(v), N, [](){
                return (std::rand());
        });
        auto w = v;

        insertion_sort(std::begin(v), std::end(v));
        selection_sort(std::begin(w), std::end(w));
        
        BOOST_CHECK(boost::algorithm::is_sorted(std::begin(v), std::end(v)));
        BOOST_CHECK(boost::algorithm::is_sorted(std::begin(w), std::end(w)));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace algorithm
}       // namespace dctl
