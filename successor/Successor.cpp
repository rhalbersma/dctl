#include <algorithm>                    // find
#include <cstddef>                      // size_t
#include <string>                       // string
#include <boost/test/unit_test.hpp> 
#include "../../src/successor/Selection.hpp"
#include "../../src/successor/Successor.hpp"
#include "../../src/node/Position.hpp"
#include "../../src/setup/Setup.hpp"
#include "../../src/notation/String.hpp"
#include "../../src/board/Types.hpp"
#include "../../src/variant/Variant.hpp"

namespace dctl {

#if INTEGRATION_TEST == 0

BOOST_AUTO_TEST_SUITE(TestSuccessor)

BOOST_AUTO_TEST_CASE(Italian)
{
        // Positions from the official Italian rules: 
        // http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
        std::string position[] = {
                "W:W30:B27",                    // Art. 5.6  (man capture directions)
                "W:W31:B12,20,28",              // Art. 5.7  (man capture continuation)
                "W:WK22:B18,19,27",             // Art. 5.8  (king capture directions)
                "W:WK27:B5,6,13,15,21,23",      // Art. 5.9  (king capture continuation)
                "W:WK21,31:B10,18,27",          // Art. 6.6  (capture most number of pieces)
                "W:WK23,31:B19,26,27",          // Art. 6.7  (capture with king)
                "W:WK23:B19,K28",               // Art. 6.8  (capture most kings)
                "W:WK30:B10,12,18,K20,K26,27",  // Art. 6.9  (capture first kings)
                "W:WK31:B18,20,K27,K28"         // Art. 6.10 (equivalent capture)
        };

        int size[] = { 1, 1, 3, 1, 1, 1, 1, 1, 2 };

        std::string legal_56[] = { "30x23" };
        std::string legal_57[] = { "31x8 " };
        std::string legal_58[] = { "22x13", "22x15", "22x31" };
        std::string legal_59[] = { "27x25" };
        std::string legal_66[] = { "31x6 " };
        std::string legal_67[] = { "23x21" };
        std::string legal_68[] = { "23x32" };
        std::string legal_69[] = { "30x5 " };
        std::string legal_610[] = { "31x13", "31x15" };

        std::string* legal[] = {
                legal_56, legal_57, legal_58, legal_59,
                legal_66, legal_67, legal_68, legal_69, legal_610
        };

        for (auto i = 0; i < 9; ++i) {
                const auto p = setup::read<board::Roman, pdn::protocol>()(position[i]);
                Stack moves;
                Successor<select::Legal, variant::Italian>::generate(p, moves);

                // check the number of generated legal moves
                BOOST_CHECK_EQUAL(size[i], static_cast<int>(moves.size()));

                // check all generated legal moves
                for (auto j = 0; j < static_cast<int>(moves.size()); ++j) {
                        const auto move_string = notation::write<variant::Italian>()(p, moves[j]);
                        BOOST_CHECK_NE(legal[i] + size[i], std::find(legal[i], legal[i] + size[i], move_string)); 
                }
        }
}

BOOST_AUTO_TEST_CASE(Spanish)
{
        // Positions from the official Italian rules: 
        // http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
        std::string position[] = {
                "W:W30:B27",                    // Art. 5.6  (man capture directions)
                "W:W31:B12,20,28",              // Art. 5.7  (man capture continuation)
                "W:WK22:B18,19,27",             // Art. 5.8  (king capture directions)
                "W:WK27:B5,6,13,15,21,23",      // Art. 5.9  (king capture continuation)
                "W:WK21,31:B10,18,27",          // Art. 6.6  (capture most number of pieces)
                "W:WK23,31:B19,26,27",          // Art. 6.7  (capture with king)
                "W:WK23:B19,K28",               // Art. 6.8  (capture most kings)
                "W:WK30:B10,12,18,K20,K26,27",  // Art. 6.9  (capture first kings)
                "W:WK31:B18,20,K27,K28"         // Art. 6.10 (equivalent capture)
        };

        int size[] = { 1, 1, 6, 2, 1, 3, 1, 1, 2 };

        std::string legal_56[] = { "30x23" };
        std::string legal_57[] = { "31x8 " };
        std::string legal_58[] = { "22x8 ", "22x12", "22x15", "22x9 ", "22x13", "22x31" };
        std::string legal_59[] = { "27x17", "27x25" };
        std::string legal_66[] = { "31x6 " };
        std::string legal_67[] = { "23x17", "23x21", "31x15" };
        std::string legal_68[] = { "23x32" };
        std::string legal_69[] = { "30x29" };
        std::string legal_610[] = { "31x21", "31x25" };

        std::string* legal[] = {
                legal_56, legal_57, legal_58, legal_59,
                legal_66, legal_67, legal_68, legal_69, legal_610
        };

        for (auto i = 0; i < 9; ++i) {
                const auto p = setup::read<board::Roman, pdn::protocol>()(position[i]);
                Stack moves;
                Successor<select::Legal, variant::Spanish>::generate(p, moves);

                // check the number of generated legal moves
                BOOST_CHECK_EQUAL(size[i], static_cast<int>(moves.size()));

                // check all generated legal moves
                for (auto j = 0; j < static_cast<int>(moves.size()); ++j) {
                        const auto move_string = notation::write<variant::Spanish>()(p, moves[j]);
                        BOOST_CHECK_NE(legal[i] + size[i], std::find(legal[i], legal[i] + size[i], move_string)); 
                }
        }
}

BOOST_AUTO_TEST_SUITE_END()

#endif

}       // namespace dctl
