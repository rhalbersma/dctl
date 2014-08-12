#include <dctl/bit.hpp>                         // Set, set_intersection, set_union, set_symmetric_difference, set_difference
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(BitAlgorithm)

using SetTypes = boost::mpl::vector
<
        Set< 64>,
        Set<128>,
        Set<192>,
        Set<256>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(SetEmpty, T, SetTypes)
{
        constexpr auto b = T{};
        BOOST_CHECK(b.empty());
        BOOST_CHECK(!set_single(b));
        BOOST_CHECK(!set_double(b));
        BOOST_CHECK(!set_multiple(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetSingle, T, SetTypes)
{
        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                auto const b = T{i};
                BOOST_CHECK(!b.empty());
                BOOST_CHECK( set_single(b));
                BOOST_CHECK(!set_double(b));
                BOOST_CHECK(!set_multiple(b));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetDouble, T, SetTypes)
{
        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                for (auto j = 0; j < std::numeric_limits<T>::digits; ++j) {
                        auto const b = T{i, j};
                        if (i == j) {
                                BOOST_CHECK(b.empty());
                        } else {
                                BOOST_CHECK(!b.empty());
                                BOOST_CHECK(!set_single(b));
                                BOOST_CHECK( set_double(b));
                                BOOST_CHECK( set_multiple(b));
                        }
                }
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetMultiple, T, SetTypes)
{
        for (std::size_t i = 0; i < static_cast<std::size_t>(std::numeric_limits<T>::digits); ++i) {
                auto const b = ~((~T{} >> i) << i);
                switch (i) {
                case 0:
                        BOOST_CHECK(b.empty());
                        break;
                case 1:
                        BOOST_CHECK(set_single(b));
                        break;
                case 2:
                        BOOST_CHECK(set_double(b));
                        BOOST_CHECK(set_multiple(b));
                        break;
                default:
                        BOOST_CHECK(!b.empty());
                        BOOST_CHECK(!set_single(b));
                        BOOST_CHECK(!set_double(b));
                        BOOST_CHECK( set_multiple(b));
                        break;
                }
        }
        BOOST_CHECK(set_multiple(~T{}));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetIntersection, T, SetTypes)
{
        constexpr auto a = T { 0, 1, 2, T::max_size() - 2, T::max_size() - 1 };
        constexpr auto b = T { 0, 1, 3, T::max_size() - 4, T::max_size() - 3 };

        constexpr auto i_ab = set_intersection(a, b);
        constexpr auto i_ba = set_intersection(b, a);
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(i_ab), end(i_ab), begin(i_ba), end(i_ba));

        BOOST_CHECK(includes(a, i_ab));
        BOOST_CHECK(includes(b, i_ab));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetUnion, T, SetTypes)
{
        constexpr auto a = T { 0, 1, 2, T::max_size() - 2, T::max_size() - 1 };
        constexpr auto b = T { 0, 1, 3, T::max_size() - 4, T::max_size() - 3 };

        constexpr auto u_ab = set_union(a, b);
        constexpr auto u_ba = set_union(b, a);
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(u_ab), end(u_ab), begin(u_ba), end(u_ba));

        BOOST_CHECK(includes(u_ab, a));
        BOOST_CHECK(includes(u_ab, b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetSymmetricDifference, T, SetTypes)
{
        constexpr auto a = T { 0, 1, 2, T::max_size() - 2, T::max_size() - 1 };
        constexpr auto b = T { 0, 1, 3, T::max_size() - 4, T::max_size() - 3 };

        constexpr auto s_ab = set_symmetric_difference(a, b);
        constexpr auto s_ba = set_symmetric_difference(b, a);
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(s_ab), end(s_ab), begin(s_ba), end(s_ba));

        constexpr auto u = set_union(set_difference(a, b), set_difference(b, a));
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(s_ab), end(s_ab), begin(u), end(u));

        constexpr auto d = set_difference(set_union(a, b), set_intersection(a, b));
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(s_ab), end(s_ab), begin(d), end(d));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetDifference, T, SetTypes)
{
        constexpr auto a = T { 0, 1, 2, T::max_size() - 2, T::max_size() - 1 };
        constexpr auto b = T { 0, 1, 3, T::max_size() - 4, T::max_size() - 3 };

        constexpr auto d_ab = set_difference(a, b);
        constexpr auto d_ba = set_difference(b, a);

        BOOST_CHECK( includes(a, d_ab));
        BOOST_CHECK(!includes(a, d_ba));
        BOOST_CHECK( includes(b, d_ba));
        BOOST_CHECK(!includes(b, d_ab));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
