#include <iostream>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/board/types.hpp>                 // International
#include <dctl/setup/diagram.hpp>               // diagram, bits, squares

namespace dctl {
namespace setup {

BOOST_AUTO_TEST_SUITE(SetupLayout)

typedef boost::mpl::vector<
        board::Micro, board::Mini, board::Checkers, board::Roman, board::International,
        board::Frisian, board::Spantsireti, board::Ktar11, board::Ktar12
> BoardSequence;

BOOST_AUTO_TEST_CASE_TEMPLATE(Layout, T, BoardSequence)
{
        std::cout << diagram<T, squares>()() << "\n";
        std::cout << diagram<T, bits>()() << "\n";
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace setup
}       // namespace dctl
