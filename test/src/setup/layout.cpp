#include <iostream>
#include <typeinfo>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/board/types.hpp>                 // international
#include <dctl/setup/diagram.hpp>               // diagram, bits, squares
#include <dctl/board/algebraic.hpp>
#include <iterator>
#include <dctl/state/state.hpp>
#include <dctl/setup/diagram.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/string.hpp>

namespace dctl {
namespace setup {

BOOST_AUTO_TEST_SUITE(SetupLayout)

using BSequence = boost::mpl::vector
<
        board::checkers,
        board::rectangular< 8, 10>,
        board::rectangular<10,  8>,
        board::international,
        board::rectangular<10, 12>,
        board::rectangular<12, 10>,
        board::compact1210,
        board::canadian,
        board::checkers,
        board::ktar< 8, 10>,
        board::ktar<10,  8>,
        board::international,
        board::ktar<10, 11>,
        board::ktar<10, 12>,
        board::compact1012,
        board::ktar<12, 10>,
        board::canadian,
        board::rectangular<10, 9>,
        board::rectangular<11, 9>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(Initial, T, BSequence)
{
        auto const p = State<rules::international, T>::initial();
        std::cout << setup::diagram<pdn::protocol>()(p);
        std::cout << "W = " << T::width() << ", H = " << T::height() << ", bits = " << T::bits() << "\n\n";
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace setup
}       // namespace dctl
