#pragma once
#include <dctl/color.hpp>       // white
#include <xstd/type_traits.hpp> // type_is

namespace dctl {

template<class TabulationHash>
auto hash_xor_accumulate(TabulationHash const& h, Color c)
{
        return h.to_move(c);
}

template<class TabulationHash, class Move>
auto hash_xor_accumulate(TabulationHash const& h, Move const& /* m */, xstd::type_is<Color>)
{
        return h.to_move(Color::white);
}

}       // namespace dctl
