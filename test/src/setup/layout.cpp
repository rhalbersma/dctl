#include <dctl/board.hpp>                       // International
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
        board::Micro,
        board::Mini,
        board::Checkers,
        board::Italian,
        board::Spantsiretti,
        board::Rectangular<10, 9>,
        board::Rectangular<11, 9>,
        board::International,
        board::Ktar<10, 11>,
        board::Ktar<10, 12>,
        board::Rectangular<12, 10>,
        board::Canadian,
        board::SriLankan,
        board::Dumm,
        board::Rectangular<19, 10>,
        board::Rectangular<27, 14>
>;

using RSequence = boost::mpl::vector
<
        rules::Checkers,
        rules::Czech,
        rules::Frisian,
        rules::International,
        rules::Italian,
        rules::Pool,
        rules::Russian,
        rules::Spanish,
        rules::Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(initial, T, BSequence)
{
        auto const p = state<rules::International, T>::initial();
        std::cout << setup::diagram<pdn::protocol>()(p);
        std::cout << "W = " << T::width << ", H = " << T::height << ", P = " << T::is_inverted << ", bits = " << T::bits() << "\n\n";
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ActionSize, T, RSequence)
{
        using A = action<T, board::International>;
        std::cout << "sizeof(Action) = " << sizeof(A) << "\n";
}

BOOST_AUTO_TEST_CASE_TEMPLATE(StateSize, T, RSequence)
{
        using S = state<T, board::International>;
        std::cout << "sizeof(State) = " << sizeof(S) << "(" << sizeof(square_t<board::International>) << ")" << "\n";
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

}       // namespace setup
}       // namespace dctl
