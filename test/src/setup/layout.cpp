#include <dctl/board.hpp>                       // international
#include <dctl/action.hpp>
#include <dctl/setup/diagram.hpp>               // diagram, bits, squares
#include <dctl/rules.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/state/state.hpp>
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
        board::spantsiretti,
        board::rectangular<10, 9>,
        board::rectangular<11, 9>,
        board::international,
        board::ktar<10, 11>,
        board::compact_10_12,
        board::compact_12_10,
        board::ktar<10, 12>,
        board::rectangular<12, 10>,
        board::canadian,
        board::srilankan,
        board::dumm
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

BOOST_AUTO_TEST_CASE(Grid)
{
        auto const d = board::detail::dimensions{1,1,true};
        auto const i = board::detail::InnerGrid{d};
        auto const o = board::detail::OuterGrid{i, 2};
        std::cout << "W = " << i.width() << ", H = " << i.height() << ", P = " << i.is_inverted() << ", bits = " << i.size() << "\n\n";
        std::cout << "W = " << o.width() << ", H = " << o.height() << ", P = " << o.is_inverted() << ", bits = " << o.size() << "\n\n";
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace maskup
}       // namespace dctl
