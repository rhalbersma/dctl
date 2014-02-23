#include <cstdint>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/bit/set.hpp>                     // Set

#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/adjacent_find.hpp>
#include <boost/range/end.hpp>
#include <dctl/egdb/index.hpp>
#include <iostream>
#include <iomanip>
#include <boost/range/irange.hpp>
#include <dctl/board/types.hpp>
#include <dctl/board/mask.hpp>

namespace dctl {
namespace egdb {

BOOST_AUTO_TEST_SUITE(EndgameDatabaseIndex)

using U = int;

using SetTypes = boost::mpl::vector
<
        bit::Set<U, uint64_t, 1>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(ColexSubsetRank, T, SetTypes)
{
        constexpr auto b = T{ 0, 1, 2 };
        constexpr auto N = T{}.max_size();
        constexpr auto e = T{ N - 2, N - 1 };

        BOOST_CHECK_EQUAL(colex_combination_rank(b), 0);
        BOOST_CHECK_EQUAL(colex_combination_rank(e), (Binomial<N, 10>::coefficient(N, e.size())) - 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ColexSubsetUnRank, T, SetTypes)
{
        using binomial = Binomial<384, 192>;
        auto N = 50; auto K = 2;

        auto b = std::ptrdiff_t{0};
        auto e = binomial::coefficient(N, K);

        using Board = board::International;
        auto pattern = ~board::Squares<Board>::mask();
        for (auto sq : pattern)
                std::cout << sq << ",";
        std::cout << "\n";

        auto x = boost::irange(0, 50) | boost::adaptors::transformed([&](auto i) {
                return i + bit::intrinsic::popcount(pattern.data() & ((1ULL << i) - 1));
        });
        for (auto j : x)
                std::cout << j << ",";

        for (auto i : boost::irange(b, e)) {
                auto pos = colex_combination_unrank<T>(i, {N,K});
                auto ipos = mixin(pos, pattern);
                auto opos = mixout(ipos, pattern);
                auto idx = colex_combination_rank(opos);
                std::cout << "   i =" <<   i << ", ";
                std::cout << " pos ="; std::copy(begin(pos), end(pos), std::ostream_iterator<int>(std::cout, ","));
                std::cout << " ipos="; boost::copy(ipos, std::ostream_iterator<int>(std::cout, ","));
                std::cout << " opos="; boost::copy(opos, std::ostream_iterator<int>(std::cout, ","));
                std::cout << " idx =" << idx << "\n";

                //BOOST_CHECK_EQUAL(idx, i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ColexSubsetUnRank2, T, SetTypes)
{
        using binomial = Binomial<384, 192>;
        auto N = 50; auto K = 1;

        auto b = std::ptrdiff_t{0};
        auto e = binomial::coefficient(N, K);

        using R = bit::InputRange<int, uint64_t, 1>;
        using Board = board::International;
        auto const pattern = T{0};//board::Squares<Board>::mask();

        for (auto i : boost::irange(b, e)) {
                auto t = colex_combination_unrank2<T>(i, {N,K});
                auto pos = mixin2<T>(R{t.data()}, pattern);
                auto idx = colex_combination_rank2_mixout(R{pos.data()}, pattern);
                BOOST_CHECK_EQUAL(idx, i);
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace egdb
}       // namespace dctl
