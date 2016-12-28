#include <dctl/aima/egdb/index.hpp>     // subdatabase
#include <dctl/board.hpp>               // international
#include <dctl/rules.hpp>               // international
#include <dctl/state.hpp>               // state
#include <dctl/utility/type_traits.hpp> // position_t
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END

using namespace dctl;

BOOST_AUTO_TEST_SUITE(Index)

BOOST_AUTO_TEST_CASE(RankIsInverseOfUnrank)
{
        using rules_type = rules::international;
        using board_type = board::mini;
        using state_type = state<rules_type, board_type>;

        auto const db = egdb::subdatabase<position_t<state_type>>(1, 1, 1, 1);
        for (auto index = 0LL; index < db.size(); ++index) {
                if (auto const p = db.unrank_position(index); !p) {
                        continue;
                } else {
                        BOOST_CHECK_EQUAL(db.rank_position(p.value()), index);
                }
        }
}

BOOST_AUTO_TEST_SUITE_END()
