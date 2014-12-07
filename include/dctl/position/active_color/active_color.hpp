#pragma once
#include <dctl/color.hpp>
#include <dctl/position/active_color/zobrist.hpp>

namespace dctl {

template<class TabulationHash>
auto hash_xor_accumulate(TabulationHash const& h, Color c)
{
        return h.color[static_cast<bool>(c)];
}

}       // namespace dctl
