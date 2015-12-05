#include <dctl/board.hpp>
#include <dctl/state.hpp>
#include <dctl/rules.hpp>
#include <dctl/ai/traversal/root.hpp>

using namespace dctl;

int main()
{
        auto const s = State<rules::International, board::International>::initial();
        auto const    unique_gen = actions::Successor<actions::select::legal, true >{};
        traversal::xperft(s, 11, unique_gen);
}
