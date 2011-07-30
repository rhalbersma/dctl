#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "../../../Library/src/setup/Diagram.h"
#include "../../../Library/src/setup/String.h"
#include "../../../Library/src/protocol/dxp/DXP.h"
#include "../../../Library/src/protocol/pdn/PDN.h"
#include "../../../Library/src/board/Types.h"

namespace dctl {
namespace setup {

TEST(Setup, Wieger)
{
        // Test position from email conversation with Wieger Wesselink
        std::string w = "O    \
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
        // with a modified character set (default is <'Z', 'W', 'E>)
        auto pos_w = setup::read<
                board::International, 
                protocol::dxp::version, 
                setup::Token<'X', 'O', '.'> 
        >()(w);

        // write the above position as a diagram and a FEN string using the PDN protocol
        auto diag_w = setup::diagram<protocol::pdn::version>()(pos_w);
        auto FEN_w = setup::write<protocol::pdn::version>()(pos_w);        

        // convert the new FEN string to a new position and back into a new FEN string
        auto pos_r = setup::read<board::International, protocol::pdn::version>()(FEN_w);
        auto FEN_r = setup::write<protocol::pdn::version>()(pos_r);

        // the two positions have been parsed through different protocols,
        // but they should have identical FEN strings 
        EXPECT_EQ(FEN_w, FEN_r);     

        std::cout << diag_w << std::endl;
        std::cout << FEN_w << std::endl;
        std::cout << FEN_r << std::endl; 
}

}       // namespace setup
}       // namespace dctl
