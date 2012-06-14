#include <boost/test/unit_test.hpp>
#include "../../src/board/Types.hpp"
#include "../../src/rules/Types.hpp"
#include "../../src/setup/Setup.hpp"

namespace dctl {
namespace setup {

BOOST_AUTO_TEST_SUITE(TestSetup)

BOOST_AUTO_TEST_CASE(Wieger)
{
        // Test position from email conversation with Wieger Wesselink
        auto const w = "O     \
            .   .   .   .   . \
          .   .   .   .   .   \
            .   .   x   x   x \
          x   .   .   x   x   \
            x   .   .   x   x \
          .   o   o   o   .   \
            .   o   o   o   o \
          .   o   .   o   o   \
            .   .   .   o   o \
          .   .   .   .   .   \
        ";

        // parse the above diagram into a position using the DamExchange protocol
        // with a modified character set (default is <'Z', 'W', 'E'>)
        auto const pos_w = read<
                rules::International,
                board::International,
                dxp::protocol,
                TokenInterface<'X', 'O', '.'>
        >()(w);

        // write the above position as a FEN string using the PDN protocol
        auto const FEN_w = write<pdn::protocol>()(pos_w);

        // convert the new FEN string to a new position and back into a new FEN string
        auto const pos_r = read<rules::International, board::International, pdn::protocol>()(FEN_w);
        auto const FEN_r = write<pdn::protocol>()(pos_r);

        // the two positions have been parsed through different protocols,
        // but they should have identical FEN strings
        BOOST_REQUIRE_EQUAL(FEN_w, FEN_r);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace setup
}       // namespace dctl
