#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/packed/packed.hpp>               // set

namespace dctl {
namespace packed {

typedef set<int> bitset;

BOOST_AUTO_TEST_SUITE(Algorithms)

BOOST_AUTO_TEST_CASE(SetIntersection)
{
        bitset const a { 0, 1, 2, 3 };
        bitset const b { 0, 1, 4, 5 };

        auto const i = set_intersection(a, b);
        BOOST_CHECK(set_includes(a, i));
        BOOST_CHECK(set_includes(b, i));

        auto const j = a & b;
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(i), end(i), begin(j), end(j));
}

BOOST_AUTO_TEST_CASE(SetUnion)
{
        bitset const a { 0, 1, 2, 3 };
        bitset const b { 0, 1, 4, 5 };

        auto const u = set_union(a, b);
        BOOST_CHECK(set_includes(u, a));
        BOOST_CHECK(set_includes(u, b));

        auto const v = a | b;
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(u), end(u), begin(v), end(v));
}

BOOST_AUTO_TEST_CASE(SetSymmetricDifference)
{
        bitset const a { 0, 1, 2, 3 };
        bitset const b { 0, 1, 4, 5 };

        auto const s = set_symmetric_difference(a, b);
        auto const t = a ^ b;
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(s), end(s), begin(t), end(t));

        auto const d_ab = set_difference(a, b);
        auto const d_ba = set_difference(b, a);
        auto const u = set_union(d_ab, d_ba);
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(s), end(s), begin(u), end(u));
}

BOOST_AUTO_TEST_CASE(SetDifference)
{
        bitset const a { 0, 1, 2, 3 };
        bitset const b { 0, 1, 4, 5 };

        auto const d_ab = set_difference(a, b);
        auto const d_ba = set_difference(b, a);

        BOOST_CHECK( set_includes(a, d_ab));
        BOOST_CHECK(!set_includes(a, d_ba));
        BOOST_CHECK( set_includes(b, d_ba));
        BOOST_CHECK(!set_includes(b, d_ab));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace packed
}       // namespace dctl




