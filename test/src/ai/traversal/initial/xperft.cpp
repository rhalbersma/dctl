#include <dctl/board.hpp>
#include <dctl/state.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/ai/traversal/root.hpp>

using namespace dctl;

int main()
{
        //auto const s = State<rules::Checkers, board::Checkers>::initial();
        //auto const non_unique_gen = core::Actions<core::select::legal, true>{};
        //traversal::sperft(s, 17, non_unique_gen);
        auto const s = State<rules::International, board::International>::initial();
        auto const unique_gen = core::Actions<core::select::legal>{};
        traversal::sperft(s, 11, unique_gen);
}
