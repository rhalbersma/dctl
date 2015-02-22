#include <walk/fixture.hpp>
#include <dctl/board/types.hpp>
#include <dctl/position.hpp>
#include <dctl/rules.hpp>
#include <dctl/walk/root.hpp>
#include <boost/test/unit_test.hpp>
#include <cstddef>                      // size_t

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(WalkInitialRectangular)

BOOST_AUTO_TEST_CASE(Spantsireti)
{
        auto const p = Position<international::Rules, board::Spantsireti>::initial();
        std::size_t const leafs[] = { 9, 81, 658, 4'265, 27'117, 167'140, 1'029'319, 6'127'190, 36'751'086, 218'398'167, 1'306'818'894 };
        test(p, unique_gen, leafs);
}

BOOST_AUTO_TEST_CASE(Ktar10x11)
{
        auto const p = Position<international::Rules, board::Ktar<10,11>>::initial();
        std::size_t const leafs[] = { 9, 81, 810, 7'583, 74'602, 688'835, 6'555'302, 59'796'721, 558'094'366, 5'058'805'542 };
        test(p, unique_gen, leafs);
}

// The "Perft on rectangular boards" thread on the FMJD forum (results until depth=9)
// http://laatste.info/bb3/viewtopic.php?f=53&t=3014
// The "Perft on large boards" thread on the FMJD forum (results until depth=11)
// http://laatste.info/bb3/viewtopic.php?f=53&t=4563&start=2

BOOST_AUTO_TEST_CASE(Ktar10x12)
{
        std::size_t const leafs[] = { 9, 81, 658, 4'265, 27'117, 167'140, 1'049'442, 6'483'961, 41'291'394, 263'895'730, 1'731'541'289 };

        // board with International initial moves and no orthogonal capture support: fits into 64 bits
        auto const p = Position<international::Rules, board::Compact1012>::initial();
        test(p, unique_gen, leafs);

        // board with International initial moves and orthogonal capture support: does not fit into 64 bits
        auto const q = Position<international::Rules, board::Ktar<10,12>>::initial();
        test(q, unique_gen, leafs);
}

// The "Perft on large boards" thread on the FMJD forum (results until depth=10)
// http://laatste.info/bb3/viewtopic.php?f=53&t=4563&start=1

BOOST_AUTO_TEST_CASE(Board12x10)
{
        std::size_t const leafs[] = { 11, 121, 1'222, 10'053, 79'049, 584'100, 4'369'366, 31'839'056, 236'364'607, 1'742'748'504 };

        // board with regular coloring and no orthogonal capture support: fits into 64 bits
        auto const p = Position<international::Rules, board::Compact1210>::initial();
        test(p, unique_gen, leafs);

        // board with Canadian initial moves and orthogonal capture support: does not fit into 64 bits
        auto const q = Position<international::Rules, board::Board<12,10>>::initial();
        test(q, unique_gen, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl
