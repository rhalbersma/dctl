#pragma once
#include <dctl/successor/enumeration/enumerator_fwd.hpp>
#include <dctl/successor/enumeration/king_moves.hpp>
#include <dctl/successor/enumeration/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct enumerator<Color, Material::both, Moves, Position>
{
private:
        // typedefs
        typedef enumerator<Color, Material::king, Moves, Position> KingMoves;
        typedef enumerator<Color, Material::pawn, Moves, Position> PawnMoves;

public:
        int operator()(Position const& p) const
        {
                return KingMoves()(p) + PawnMoves()(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl