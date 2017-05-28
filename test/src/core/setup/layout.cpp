#include <dctl/core/board.hpp>                       // international
#include <dctl/core/action.hpp>
#include <dctl/core/setup/diagram.hpp>               // diagram, bits, squares
#include <dctl/core/rules.hpp>
#include <dctl/core/setup/string.hpp>
#include <dctl/core/state/state.hpp>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <iostream>
#include <iterator>
#include <typeinfo>

namespace dctl {
namespace setup {

BOOST_AUTO_TEST_SUITE(SetupLayout)

using BSequence = boost::mpl::vector
<
        board::micro,
        board::mini,
        board::checkers,
        board::italian,
        board::spantsiretti,
        board::rectangular<10, 9>,
        board::rectangular<11, 9>,
        board::international,
        board::ktar<10, 11>,
        board::ktar<10, 12>,
        board::rectangular<12, 10>,
        board::canadian,
        board::srilankan,
        board::dumm,
        board::rectangular<19, 10>,
        board::rectangular<27, 14>
>;

using RSequence = boost::mpl::vector
<
        rules::checkers,
        rules::czech,
        rules::frisian,
        rules::international,
        rules::italian,
        rules::pool,
        rules::russian,
        rules::spanish,
        rules::thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(initial, T, BSequence)
{
        auto const p = state<rules::international, T>::initial();
        std::cout << setup::diagram<pdn::protocol>()(p);
        std::cout << "W = " << T::width << ", H = " << T::height << ", P = " << T::is_inverted << ", bits = " << T::bits() << "\n\n";
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ActionSize, T, RSequence)
{
        using A = action<T, board::international>;
        std::cout << "sizeof(Action) = " << sizeof(A) << "\n";
}

BOOST_AUTO_TEST_CASE_TEMPLATE(StateSize, T, RSequence)
{
        using S = state<T, board::international>;
        std::cout << "sizeof(State) = " << sizeof(S) << "(" << sizeof(square_t<board::international>) << ")" << "\n";
}

#define TEST_CPU_SUPPORT(m) std::cout << "This machine has support for " << #m << ": " << __builtin_cpu_supports(#m) << '\n';

BOOST_AUTO_TEST_CASE(Grid)
{
	__builtin_cpu_init();
	TEST_CPU_SUPPORT(cmov)
	TEST_CPU_SUPPORT(mmx)
	TEST_CPU_SUPPORT(popcnt)
	TEST_CPU_SUPPORT(sse)
	TEST_CPU_SUPPORT(sse2)
	TEST_CPU_SUPPORT(sse3)
	TEST_CPU_SUPPORT(ssse3)
	TEST_CPU_SUPPORT(sse4.1)
	TEST_CPU_SUPPORT(sse4.2)
	TEST_CPU_SUPPORT(avx)
	TEST_CPU_SUPPORT(avx2)
	TEST_CPU_SUPPORT(avx512f)
        auto const d = board::detail::dimensions{1,1,true};
        auto const i = board::detail::InnerGrid{d};
        auto const o = board::detail::OuterGrid{i, 2};
        std::cout << "W = " << i.width() << ", H = " << i.height() << ", P = " << i.is_inverted() << ", bits = " << i.size() << "\n\n";
        std::cout << "W = " << o.width() << ", H = " << o.height() << ", P = " << o.is_inverted() << ", bits = " << o.size() << "\n\n";
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace setup
}       // namespace dctl
