#include <dctl/bit.hpp>                         // Set
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(BitIterator)

using SetTypes = boost::mpl::vector
<
        Set< 64>,
        Set<128>,
        Set<192>,
        Set<256>
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
