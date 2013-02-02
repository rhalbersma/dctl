#pragma once
#include <dctl/successor/detect/detector_fwd.hpp>
#include <dctl/successor/detect/king_reverse.hpp>
#include <dctl/successor/detect/pawn_reverse.hpp>
#include <dctl/successor/select/reverse.hpp>
#include <dctl/node/material.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position, typename Range>
struct detector<Color, Material::both, Reverse, Position, Range>
{
private:
        // typedefs

        // the implementation of both pawn and king move detection is independent of Range,
        // but we explicitly pass rules::range::distance_1 here to avoid code bloat
        typedef detector<Color, Material::pawn, Reverse, Position, rules::range::distance_1> PawnMoves;
        typedef detector<Color, Material::king, Reverse, Position, rules::range::distance_1> KingMoves;

public:
        bool operator()(Position const& p) const
        {
                // speculate #pawns > #kings so that the || is likely to short-circuit
                return PawnMoves()(p) || KingMoves()(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
