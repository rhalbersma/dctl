#pragma once
#include <dctl/color.hpp>      // white
#include <xstd/type_traits.hpp> // type_is

namespace dctl {

template<class TabulationHash>
auto hash_xor_accumulate(TabulationHash const& h, color p)
{
        return h.player(p);
}

template<class TabulationHash, class Action>
auto hash_xor_accumulate(TabulationHash const& h, Action const& /* m */, xstd::type_is<color>)
{
        return h.player(color::white);
}

}       // namespace dctl
