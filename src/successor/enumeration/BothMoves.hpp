#pragma once
#include "Enumerator_fwd.hpp"
#include "KingMoves.hpp"
#include "PawnMoves.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct enumerator<Color, Material::both, select::Moves, Position>
{
private:
        // typedefs
        typedef enumerator<Color, Material::king, select::Moves, Position> KingMoves;
        typedef enumerator<Color, Material::pawn, select::Moves, Position> PawnMoves;

public:
        int operator()(Position const& p)
        {
                return (
                        KingMoves()(p) +
                        PawnMoves()(p)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
