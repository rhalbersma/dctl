#include <dctl/board/types.hpp>
#include <dctl/state.hpp>
#include <dctl/rules.hpp>
#include <dctl/walk/root.hpp>

using namespace dctl;

int main()
{
        auto const s = State<rules::International, board::International>::initial();
        auto const    unique_gen = actions::Successor<actions::select::legal, true >{};
        walk::xperft(s, 11, unique_gen);
}
