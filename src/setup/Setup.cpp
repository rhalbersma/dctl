#include <boost/test/unit_test.hpp> 
#include "../../../DCTL/src/board/Types.h"
#include "../../../DCTL/src/setup/Setup.h"

namespace dctl {
namespace setup {

BOOST_AUTO_TEST_SUITE(TestSetup)

BOOST_AUTO_TEST_CASE(Wieger)
{
        // Test position from email conversation with Wieger Wesselink
        auto w = "O    \
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
        auto pos_w = setup::read<
                board::International, 
                dxp::protocol, 
                setup::TokenSetBase<'X', 'O', '.'> 
        >()(w);

        // write the above position as a diagram and a FEN string using the PDN protocol
        auto diag_w = setup::diagram<pdn::protocol>()(pos_w);
        auto FEN_w = setup::write<pdn::protocol>()(pos_w);        

        // convert the new FEN string to a new position and back into a new FEN string
        auto pos_r = setup::read<board::International, pdn::protocol>()(FEN_w);
        auto FEN_r = setup::write<pdn::protocol>()(pos_r);

        // the two positions have been parsed through different protocols,
        // but they should have identical FEN strings 
        BOOST_REQUIRE_EQUAL(FEN_w, FEN_r);     
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace setup
}       // namespace dctl
