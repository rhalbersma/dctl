#include <dctl/bit.hpp>                         // Set
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
        BOOST_CHECK( b.none());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetSingle, T, SetTypes)
{
        for (auto i = 0; i < T::size(); ++i) {
                auto const b = make_set<T::size()>({i});
                BOOST_CHECK(b.any());
                BOOST_CHECK(set_single(b));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetDouble, T, SetTypes)
{
        for (auto i = 0; i < T::size(); ++i) {
                for (auto j = 0; j < T::size(); ++j) {
                        auto const b = make_set<T::size()>({i, j});
                        if (i == j) {
                                BOOST_CHECK(set_single(b));
                        } else {
                                BOOST_CHECK(set_double(b));
                                BOOST_CHECK(set_multiple(b));
                        }
                }
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetMultiple, T, SetTypes)
{
        for (auto i = 0; i < T::size(); ++i) {
                auto const b = ~((~T{} >> i) << i);
                switch (i) {
                case 0:
                        BOOST_CHECK(b.none());
                        break;
                case 1:
                        BOOST_CHECK(set_single(b));
                        break;
                case 2:
                        BOOST_CHECK(set_double(b));
                        BOOST_CHECK(set_multiple(b));
                        break;
                default:
                        BOOST_CHECK(set_multiple(b));
                        break;
                }
        }
        BOOST_CHECK(set_multiple(~T{}));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetIntersection, T, SetTypes)
{
        constexpr auto a = make_set<T::size()>({ 0, 1, 2, T::size() - 2, T::size() - 1 });
        constexpr auto b = make_set<T::size()>({ 0, 1, 3, T::size() - 4, T::size() - 3 });

        constexpr auto i_ab = a & b;
        constexpr auto i_ba = b & a;
        static_assert(i_ab == i_ba, "");
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(i_ab), end(i_ab), begin(i_ba), end(i_ba));

        static_assert(i_ab.is_subset_of(a), "");
        static_assert(i_ab.is_subset_of(b), "");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetUnion, T, SetTypes)
{
        constexpr auto a = make_set<T::size()>({ 0, 1, 2, T::size() - 2, T::size() - 1 });
        constexpr auto b = make_set<T::size()>({ 0, 1, 3, T::size() - 4, T::size() - 3 });

        constexpr auto u_ab = a | b;
        constexpr auto u_ba = b | a;
        static_assert(u_ab == u_ba, "");
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(u_ab), end(u_ab), begin(u_ba), end(u_ba));

        static_assert(a.is_subset_of(u_ab), "");
        static_assert(b.is_subset_of(u_ab), "");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetSymmetricDifference, T, SetTypes)
{
        constexpr auto a = make_set<T::size()>({ 0, 1, 2, T::size() - 2, T::size() - 1 });
        constexpr auto b = make_set<T::size()>({ 0, 1, 3, T::size() - 4, T::size() - 3 });

        constexpr auto s_ab = a ^ b;
        constexpr auto s_ba = b ^ a;
        static_assert(s_ab == s_ba, "");
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(s_ab), end(s_ab), begin(s_ba), end(s_ba));

        constexpr auto u = (a - b) | (b - a);
        static_assert(s_ab == u, "");
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(s_ab), end(s_ab), begin(u), end(u));

        constexpr auto d = (a | b) - (a & b);
        static_assert(s_ab == d, "");
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(s_ab), end(s_ab), begin(d), end(d));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetDifference, T, SetTypes)
{
        constexpr auto a = make_set<T::size()>({ 0, 1, 2, T::size() - 2, T::size() - 1 });
        constexpr auto b = make_set<T::size()>({ 0, 1, 3, T::size() - 4, T::size() - 3 });

        constexpr auto d_ab = a - b;
        constexpr auto d_ba = b - a;

        static_assert( d_ab.is_subset_of(a), "");
        static_assert(!d_ba.is_subset_of(a), "");
        static_assert( d_ba.is_subset_of(b), "");
        static_assert(!d_ab.is_subset_of(b), "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
