#pragma once
#include <dctl/color_piece.hpp>      // white
#include <xstd/type_traits.hpp> // type_is

namespace dctl {

template<class TabulationHash>
auto hash_xor_accumulate(TabulationHash const& h, Color p)
{
        return h.player(p);
}

template<class TabulationHash, class Action>
auto hash_xor_accumulate(TabulationHash const& h, Action const& /* m */, xstd::type_is<Color>)
{
        return h.player(Color::white);
}

}       // namespace dctl
