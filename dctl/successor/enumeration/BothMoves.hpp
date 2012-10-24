#pragma once
#include <functional>                   // function
#include <dctl/successor/enumeration/Enumerator_fwd.hpp>
#include <dctl/successor/enumeration/KingMoves.hpp>
#include <dctl/successor/enumeration/PawnMoves.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/node/Material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct enumerator<Color, Material::both, Moves, Position>
:
        public std::function<int(Position const&)>
{
private:
        // typedefs
        typedef enumerator<Color, Material::king, Moves, Position> KingMoves;
        typedef enumerator<Color, Material::pawn, Moves, Position> PawnMoves;

public:
        int operator()(Position const& p) const
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
