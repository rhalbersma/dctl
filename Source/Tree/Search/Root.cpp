#include <utility>
#include "gtest/gtest.h"
#include "../../../test_config.h"
#include "../../../../Library/Source/Tree/Search/Root.h"
#include "../../../../Library/Source/Tree/Node/Position.h"
#include "../../../../Library/Source/Tree/Node/Protocol.h"
#include "../../../../Library/Source/Tree/Node/String.h"
#include "../../../../Library/Source/Geometry/Board.h"
#include "../../../../Library/Source/Geometry/Layout.h"
#include "../../../../Library/Source/Variants/Rules.h"

namespace tree {
namespace search {

// The fixture for testing class SearchEndgame.
class SearchEndgame: public ::testing::Test {
protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        SearchEndgame() {
        // You can do set-up work for each test here.
        }

        virtual ~SearchEndgame() {
        // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
        }

        virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
}

// Objects declared here can be used by all tests in the test case for Foo.
};

#if INTEGRATION_TEST == 1

// http://www.xs4all.nl/~mdgsoft/draughts/stats/index.html
TEST_F(SearchEndgame, International11)
{
        typedef std::pair<std::string, size_t> DB_unittest;
        int value;       
        
        DB_unittest DB_win11[] = {
                DB_unittest("W:W33:B2."  , 17),         // 1010
                DB_unittest("W:WK42:B22.",  7),         // 0110
                DB_unittest("W:W10:BK46.",  3),         // 1001
                DB_unittest("W:WK10:BK5.",  3)          // 0101
        };
                
        for (int i = 0; i < 4; ++i) {
                value = Root::analyze<variants::International>(node::string::read<geometry::International, node::FEN_tag>()(DB_win11[i].first), DB_win11[i].second);
                EXPECT_EQ(value, value::win(DB_win11[i].second));
        }
}

TEST_F(SearchEndgame, International21)
{
        typedef std::pair<std::string, size_t> DB_unittest;
        int value;       
        
        DB_unittest DB_win21[] = {
                DB_unittest("W:W40,44:B3."   , 23),     // 2010
                DB_unittest("W:WK2,11:B45."  , 11),     // 1110
                DB_unittest("W:WK7,K30:B45." ,  9),     // 0210
                DB_unittest("W:W6,7:BK45."   ,  7),     // 2001
                DB_unittest("W:WK2,6:BK50."  ,  9),     // 1101
                DB_unittest("W:WK1,K7:BK50." ,  7),     // 0201
                DB_unittest("W:W21:B3,4."    , 33),     // 1020
                DB_unittest("W:W23:B5,6"     , 29),     // David & Goliath    
                DB_unittest("W:WK38:B4,18."  , 27),     // 0120
                DB_unittest("W:W18:B5,K23."  , 17),     // 1011
                DB_unittest("W:WK26:B23,K42.",  9),     // 0111
                DB_unittest("W:W14:BK10,K46.",  3),     // 1002
                DB_unittest("W:WK10:BK5,K23.",  3)      // 0102
        };

        for (int i = 0; i < 13; ++i) {
                value = Root::analyze<variants::International>(node::string::read<geometry::International, node::FEN_tag>()(DB_win21[i].first), DB_win21[i].second);
                EXPECT_EQ(value, value::win(DB_win21[i].second));
        }
}        

TEST_F(SearchEndgame, International22)
{
        typedef std::pair<std::string, size_t> DB_unittest;
        int value;       
        
        DB_unittest DB_win22[] = {
                DB_unittest("W:W33,46:B4,5."     , 39), // 2020
                DB_unittest("W:W8,K50:B3,32."    , 27), // 1120
                DB_unittest("W:WK1,K23:B4,38."   , 25), // 0220
                DB_unittest("W:W17,35:B3,K21."   , 23), // 2011
                DB_unittest("W:WK1,12:B16,K50."  , 19), // 1111
                DB_unittest("W:WK1,K16:BK17,26." , 19), // 0211
                DB_unittest("W:W6,12:BK7,K45."   ,  7), // 2002
                DB_unittest("W:W6,K22:BK17,K50." ,  9), // 1102
                DB_unittest("W:WK6,K22:BK17,K50.",  9)  // 0202
        };

        for (int i = 0; i < 9; ++i) {
                value = Root::analyze<variants::International>(node::string::read<geometry::International, node::FEN_tag>()(DB_win22[i].first), DB_win22[i].second);
                EXPECT_EQ(value, value::win(DB_win22[i].second));
        }        
}

TEST_F(SearchEndgame, International31)
{
        typedef std::pair<std::string, size_t> DB_unittest;
        int value;       
        
        DB_unittest DB_win31[] = {
                DB_unittest("W:W12,13,16:B30."    , 23),        // 3010
                DB_unittest("W:WK4,16,18:B34."    , 21),        // 2110
                DB_unittest("W:WK8,11,K13:B44."   , 19),        // 1210
                DB_unittest("W:WK8,K20,K37:B43."  , 17),        // 0310
                DB_unittest("W:W6,10,49:BK40."    , 13),        // 3001
                DB_unittest("W:W6,K49,50:BK16."   , 29),        // 2101
                DB_unittest("W:WK40,K45,50:BK11." , 31),        // 1201
                DB_unittest("W:WK6,K22,K27:BK49." , 15),        // 0301
                DB_unittest("W:W6:B2,3,4."        , 43),        // 1030
                DB_unittest("W:WK1:B2,3,4."       , 43),        // 0130
                DB_unittest("W:W6:B3,4,K11."      , 33),        // 1021
                DB_unittest("W:WK14:B1,K10,12."   , 29),        // 0121
                DB_unittest("W:W8:B5,K13,K23."    , 17),        // 1012
                DB_unittest("W:WK1:BK18,24,K44."  ,  9),        // 0112
                DB_unittest("W:W23:BK10,K19,K46." ,  3),        // 1003
                DB_unittest("W:WK49:BK23,K43,K46.",  3)         // 0103       
        };
        
        for (int i = 0; i < 16; ++i) {
                value = Root::analyze<variants::International>(node::string::read<geometry::International, node::FEN_tag>()(DB_win31[i].first), DB_win31[i].second);
                EXPECT_EQ(value, value::win(DB_win31[i].second));
        }
}

// http://www.xs4all.nl/~mdgsoft/draughts/stats/kill-index.html        
TEST_F(SearchEndgame, Killer11)
{
        typedef std::pair<std::string, size_t> DB_unittest;                
        int value;

        DB_unittest DB_win11[] = {
                DB_unittest("W:W31:B5."   , 17),        // 1010
                DB_unittest("W:WK49:B29." ,  7),        // 0110
                DB_unittest("W:W10:BK46." ,  3),        // 1001
                DB_unittest("W:WK10:BK46.",  3)         // 0101
        };

        for (int i = 0; i < 4; ++i) {
                value = Root::analyze<variants::Killer>(node::string::read<geometry::International, node::FEN_tag>()(DB_win11[i].first), DB_win11[i].second);
                EXPECT_EQ(value, value::win(DB_win11[i].second));
        }
}

TEST_F(SearchEndgame, Killer21)
{
        typedef std::pair<std::string, size_t> DB_unittest;                
        int value;

        DB_unittest DB_win21[] = {
                DB_unittest("W:W41,46:B24."  , 63),     // 2010
                DB_unittest("B:W23:B5,6"     , 57),     // David & Goliath with black to move    
                DB_unittest("W:WK37,46:B41." , 47),     // 1110
                DB_unittest("W:WK7,K30:B45." ,  9),     // 0210
                DB_unittest("W:W21,46:BK47." , 57),     // 2001
                DB_unittest("W:WK1,46:BK47." , 49),     // 1101
                DB_unittest("W:WK5,K14:BK33.", 15),     // 0201
                DB_unittest("W:W21:B3,4."    , 33),     // 1020
                DB_unittest("W:W23:B5,6"     , 29),     // David & Goliath    
                DB_unittest("W:WK38:B3,18."  , 27),     // 0120
                DB_unittest("W:W18:B5,K23."  , 17),     // 1011
                DB_unittest("W:WK16:B24,K43.",  9),     // 0111
                DB_unittest("W:W14:BK10,K46.",  3),     // 1002
                DB_unittest("W:WK10:BK5,K41.",  3)      // 0102
        };       
        
        for (int i = 0; i < 14; ++i) {
                value = Root::analyze<variants::Killer>(node::string::read<geometry::International, node::FEN_tag>()(DB_win21[i].first), DB_win21[i].second);
                EXPECT_EQ(value, value::win(DB_win21[i].second));
        }
}

TEST_F(SearchEndgame, Killer22)
{
        typedef std::pair<std::string, size_t> DB_unittest;                
        int value;

        DB_unittest DB_win22[] = {
                DB_unittest("W:W31,49:B9,14."    , 77), // 2020
                DB_unittest("W:WK4,36:B7,38."    , 67), // 1120
                DB_unittest("W:WK29,K47:B39,44." , 27), // 0220
                DB_unittest("W:W37,42:B20,K41."  , 65), // 2011
                DB_unittest("W:W31,K38:B7,K36."  , 67), // 1111
                DB_unittest("W:WK17,K39:BK21,26.", 31), // 0211
                DB_unittest("W:W6,46:BK11,K31."  , 57), // 2002
                DB_unittest("W:WK37,46:BK41,K43.", 53), // 1102
                DB_unittest("W:WK5,K23:BK19,K50.", 17)  // 0202
        };

        for (int i = 0; i < 9; ++i) {
                value = Root::analyze<variants::Killer>(node::string::read<geometry::International, node::FEN_tag>()(DB_win22[i].first), DB_win22[i].second);
                EXPECT_EQ(value, value::win(DB_win22[i].second));
        }
}

TEST_F(SearchEndgame, Killer31)
{
        typedef std::pair<std::string, size_t> DB_unittest;                
        int value;

        DB_unittest DB_win31[] = {
                DB_unittest("W:W37,43,46:B44."    , 69),        // 3010
                DB_unittest("W:WK37,41,46:B36."   , 47),        // 2110
                DB_unittest("W:WK14,K20,33:B44."  , 15),        // 1210
                DB_unittest("W:WK7,K9,K40:B41."   ,  9),        // 0310
                DB_unittest("W:W46,49,50:BK36."   , 73),        // 3001
                DB_unittest("W:WK19,24,46:BK15."  , 51),        // 2101
                DB_unittest("W:WK11,K19,43:BK2."  , 43),        // 1201
                DB_unittest("W:WK12,K37,K43:BK48.", 13),        // 0301
                DB_unittest("W:W6:B3,4,5."        , 43),        // 1030
                DB_unittest("W:WK1:B2,4,5."       , 43),        // 0130
                DB_unittest("W:W7:B3,4,K11."      , 33),        // 1021
                DB_unittest("W:WK16:B4,13,K32."   , 29),        // 0121
                DB_unittest("W:W6:B5,K11,K22."    , 17),        // 1012
                DB_unittest("W:WK1:B24,K29,K44."  ,  9),        // 0112
                DB_unittest("W:W23:BK10,K19,K46." ,  3),        // 1003
                DB_unittest("W:WK48:BK10,K42,K46.",  3)         // 0103       
        };
        
        for (int i = 0; i < 16; ++i) {
                value = Root::analyze<variants::Killer>(node::string::read<geometry::International, node::FEN_tag>()(DB_win31[i].first), DB_win31[i].second);
                EXPECT_EQ(value, value::win(DB_win31[i].second));
        }

        //Position<geometry::International> Walinga(node::string::read<FEN_tag, geometry::International>()("W:WK46,28:BK43"));
        //Root::analyze<FrisianRules>(Walinga, 39);
}

#endif

}       // namespace search
}       // namespace tree
