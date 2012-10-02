#pragma once
#include <functional>                   // function
#include "Generator_fwd.hpp"
#include "KingReverse.hpp"
#include "PawnReverse.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Reverse, Position>
:
        public std::function<void(Position const&, Stack&)>
{
private:
        // typedefs

        typedef generator<Color, Material::king, Reverse, Position> KingMoves;
        typedef generator<Color, Material::pawn, Reverse, Position> PawnMoves;

public:
        void operator()(Position const& p, Stack& moves) const
        {
                KingMoves()(p, moves);
                PawnMoves()(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
