#pragma once
#include <functional>                   // function
#include "Generator_fwd.hpp"
#include "PawnMoves.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"
#include "../../node/Promotion.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, typename Position>
struct generator<Color, Material::pawn, Promotions, Position>
:
        public std::function<void(Position const&, Stack&)>
{
private:
        // typedefs

        typedef generator<Color, Material::pawn, Moves, Position> PawnMoves;

public:
        void operator()(Position const& p, Stack& moves) const
        {
                if (auto const active_promotors = promoting_pawns<Color>(p))
                        PawnMoves().select(active_promotors, not_occupied(p), moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
