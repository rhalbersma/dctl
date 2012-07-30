#pragma once
#include "Enumerator_fwd.hpp"
#include "KingMoves.hpp"
#include "PawnMoves.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct enumerator<Color, Material::both, select::Moves, Position>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs
        typedef enumerator<Color, Material::king, select::Moves, Position> KingMoves;
        typedef enumerator<Color, Material::pawn, select::Moves, Position> PawnMoves;

public:
        static int run(Position const& p)
        {
                return (
                        KingMoves::run(p) +
                        PawnMoves::run(p)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
