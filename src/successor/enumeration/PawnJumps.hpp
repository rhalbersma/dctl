#pragma once
#include <functional>                   // function
#include "Enumerator_fwd.hpp"
#include "../Select.hpp"
#include "../generation/PawnJumps.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn jumps enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::pawn, Jumps, Position>
:
        public std::function<int(Position const&)>
{
private:
        // typedefs

        typedef generator<Color, Material::pawn, Jumps, Position> PawnJumps;

public:
        int operator()(Position const& p) const
        {
                Stack moves;
                PawnJumps()(p, moves);
                return static_cast<int>(moves.size());
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
