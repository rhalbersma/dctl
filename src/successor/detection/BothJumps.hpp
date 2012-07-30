#pragma once
#include "Detector_fwd.hpp"
#include "KingJumps.hpp"
#include "PawnJumps.hpp"
#include "../Select.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct detector<Color, Material::both, select::Jumps, Position>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef detector<Color, Material::king, select::Jumps, Position> KingJumps;
        typedef detector<Color, Material::pawn, select::Jumps, Position> PawnJumps;

public:
        static bool run(Position const& p)
        {
                // speculate #pawns > #kings so that the || is likely to short-circuit
                return (
                        PawnJumps::run(p) ||
                        KingJumps::run(p)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
