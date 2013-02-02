#pragma once
#include <dctl/successor/count/enumerator_fwd.hpp>
#include <dctl/successor/count/king_moves.hpp>
#include <dctl/successor/count/pawn_moves.hpp>
#include <dctl/successor/select/reverse.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct enumerator<Color, Material::both, select::reverse, Position>
{
private:
        // typedefs
        typedef enumerator<Color, Material::king, select::reverse, Position> KingMoves;
        typedef enumerator<Color, Material::pawn, select::reverse, Position> PawnMoves;

public:
        int operator()(Position const& p) const
        {
                return KingMoves()(p) + PawnMoves()(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
