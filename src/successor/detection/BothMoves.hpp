#pragma once
#include "Detector_fwd.hpp"
#include "KingJumps.hpp"
#include "PawnJumps.hpp"
#include "../Select.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct detector<Color, Material::both, Moves, Position>
{
private:
        // typedefs

        typedef detector<Color, Material::king, Moves, Position> KingMoves;
        typedef detector<Color, Material::pawn, Moves, Position> PawnMoves;

public:
        bool operator()(Position const& p)
        {
                // speculate #pawns > #kings so that the || is likely to short-circuit
                return (
                        PawnMoves()(p) ||
                        KingMoves()(p)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
