#pragma once
#include <functional>                   // function
#include <dctl/successor/enumeration/Enumerator_fwd.hpp>
#include <dctl/successor/enumeration/PawnMoves.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/node/Material.hpp>
#include <dctl/node/Promotion.hpp>
#include <dctl/node/UnaryProjections.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, typename Position>
struct enumerator<Color, Material::pawn, Promotions, Position>
:
        public std::function<int(Position const&)>
{
private:
        // typedefs

        typedef enumerator<Color, Material::pawn, Moves, Position> PawnMoves;

public:
        int operator()(Position const& p) const
        {
                if (auto const active_promotors = promoting_pawns<Color>(p))
                        return (PawnMoves().select(active_promotors, not_occupied(p)));
                else
                        return (0);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
