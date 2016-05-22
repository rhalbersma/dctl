#include <dctl/board.hpp>
#include <dctl/state.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/aima/traversal/root.hpp>

using namespace dctl;

int main()
{
        //auto const s = State<rules::checkers, board::checkers>::initial();
        //auto const non_drop_duplicates_gen = core::Actions<core::select::legal, true>{};
        //traversal::sperft(s, 17, non_drop_duplicates_gen);
        auto const s = State<rules::international, board::international>::initial();
        auto const drop_duplicates_gen = core::Actions<core::select::legal>{};
        aima::traversal::sperft(s, 11, drop_duplicates_gen);
}
