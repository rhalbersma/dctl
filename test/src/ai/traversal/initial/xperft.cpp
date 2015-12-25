#include <dctl/board.hpp>
#include <dctl/state.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/ai/traversal/root.hpp>

using namespace dctl;

int main()
{
        auto const s = State<rules::Checkers, board::Checkers>::initial();
        //auto const     unique_gen = core::Actions<core::select::legal>{};
        auto const non_unique_gen = core::Actions<core::select::legal, true>{};
        traversal::sperft(s, 17, non_unique_gen);
}
