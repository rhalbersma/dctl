#include <dctl/board.hpp>
#include <dctl/state.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/aima/traversal/root.hpp>

using namespace dctl;

int main()
{
        //auto const s = state<rules::Checkers, board::Checkers>::initial();
        //auto const non_drop_duplicates_gen = Actions<select::legal, true>{};
        //traversal::sperft(s, 17, non_drop_duplicates_gen);

        using S = state<rules::International, board::International>;

        auto const states = std::vector<S>{
                S::initial(),
                setup::read<rules::International, board::International, pdn::protocol>()
                        ("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"),
                setup::read<rules::International, board::International, pdn::protocol>()
                        ("W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38")
        };

        auto const depths = std::vector<int>{ 11, 9, 15 };

        auto const drop_duplicates_gen = Actions<select::legal>{};
        for (auto i = std::size_t{0}; i < 3; ++i)
                aima::traversal::sperft(states[i], depths[i], drop_duplicates_gen);
}
