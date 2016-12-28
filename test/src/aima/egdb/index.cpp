#include <dctl/aima/egdb/index.hpp>     // subdatabase
#include <dctl/board.hpp>               // rectangular
#include <dctl/rules.hpp>               // international
#include <dctl/state.hpp>               // state
#include <dctl/utility/type_traits.hpp> // position_t
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <vector>                       // vector

using namespace dctl;

BOOST_AUTO_TEST_SUITE(Index)

BOOST_AUTO_TEST_CASE(RankIsInverseOfUnrank)
{
        using rules_type = rules::international;
        using board_type = board::rectangular<6, 6>;
        using state_type = state<rules_type, board_type>;

        using db_type = egdb::subdatabase<position_t<state_type>>;
        auto const dbs = std::vector<db_type>
        {
                { 4, 0, 0, 0 },
                { 0, 4, 0, 0 },
                { 0, 0, 4, 0 },
                { 0, 0, 0, 4 },
                { 2, 2, 0, 0 },
                { 2, 0, 2, 0 },
                { 2, 0, 0, 2 },
                { 0, 2, 2, 0 },
                { 0, 2, 0, 2 },
                { 0, 0, 2, 2 },
                { 1, 1, 1, 1 }
        };

        for (auto const& db : dbs) {
                for (auto index = 0LL; index < db.size(); ++index) {
                        if (auto const p = db.unrank_position(index)) {
                                BOOST_CHECK_EQUAL(db.rank_position(p.value()), index);
                        }
                }
        }
}

BOOST_AUTO_TEST_SUITE_END()
