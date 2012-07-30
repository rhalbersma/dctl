#pragma once
#include "Enumerator_fwd.hpp"
#include "../Select.hpp"
#include "../generation/PawnJumps.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {
        
// partial specialization for pawn jumps enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::pawn, select::Jumps, Position>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef generator<Color, Material::pawn, select::Jumps, Position> PawnJumps;

public:
        static int run(Position const& p)
        {
                Stack moves;
                PawnJumps::run(p, moves);
                return static_cast<int>(moves.size());
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
