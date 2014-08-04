#include <cstdint>                              // uint64_t
#include <functional>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/bit.hpp>                         // Set

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(BitIterator)

using U = int;

using SetTypes = boost::mpl::vector
<
        Set<U, std::less<>, uint64_t, 1>,
        Set<U, std::less<>, uint64_t, 2>,
        Set<U, std::less<>, uint64_t, 3>,
        Set<U, std::less<>, uint64_t, 4>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(ForwardIterationTraversesRange, T, SetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T::max_size() - 2, T::max_size() - 1 };
        auto it = begin(b); for (; it != end(b); ++it){}

        BOOST_CHECK(it == rbegin(b).base());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(BackwardIterationTraversesRange, T, SetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T::max_size() - 2, T::max_size() - 1 };
        auto it = end(b); for (; it != begin(b); --it){}

        BOOST_CHECK(it == rend(b).base());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ReverseForwardIterationTraversesRange, T, SetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T::max_size() - 2, T::max_size() - 1 };
        auto it = rbegin(b); for (; it != rend(b); ++it){}

        BOOST_CHECK(it.base() == begin(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ReverseBackwardIterationTraversesRange, T, SetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T::max_size() - 2, T::max_size() - 1 };
        auto it = rend(b); for (; it != rbegin(b); --it){}

        BOOST_CHECK(it.base() == end(b));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
