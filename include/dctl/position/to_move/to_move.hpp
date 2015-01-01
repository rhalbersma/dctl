#pragma once
#include <dctl/color.hpp>
#include <dctl/position/to_move/zobrist.hpp>

namespace dctl {

template<class TabulationHash>
auto hash_xor_accumulate(TabulationHash const& h, Color c)
{
        return h.to_move(c);
}

}       // namespace dctl
