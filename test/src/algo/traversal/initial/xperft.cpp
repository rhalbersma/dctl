#include <dctl/core/board.hpp>
#include <dctl/core/state.hpp>
#include <dctl/core/rules.hpp>
#include <dctl/core/setup/setup.hpp>
#include <dctl/algo/traversal/root.hpp>
#include <cstddef>

using namespace dctl;

int main()
{
        //auto const s = state<rules::checkers, board::checkers>::initial();
        //auto const non_drop_duplicates_gen = Actions<select::legal, true>{};
        //traversal::sperft(s, 17, non_drop_duplicates_gen);

        using S = state<rules::international, board::international>;

        auto const states = std::vector<S>{
                S::initial(),
                setup::read<rules::international, board::international, pdn::protocol>()
                        ("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"),
                setup::read<rules::international, board::international, pdn::protocol>()
                        ("W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38")/*
                setup::read<rules::international, board::international, pdn::protocol>()
                        ("W:WK31,K32,K33,K34,K35,K36,K37,K38,K39,K40,K41,K42,K43,K44,K45,K46,K47,K48,K49,K50:BK1,K2,K3,K4,K5,K6,K7,K8,K9,K10,K11,K12,K13,K14,K15,K16,K17,K18,K19,K20"),
                setup::read<rules::international, board::international, pdn::protocol>()
                        ("W:W6,7,8,9,10:B41,42,43,44,45")*/
        };

        auto const depths = std::vector<int>{ 11, 9, 15 /* 11, 9*/ };

        auto const drop_duplicates_gen = Actions<select::legal>{};
        for (auto i = std::size_t{0}; i < states.size(); ++i)
                aima::traversal::sperft(drop_duplicates_gen, states[i], depths[i]);
}
