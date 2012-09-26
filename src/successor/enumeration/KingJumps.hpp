#pragma once
#include <functional>                   // function
#include "Enumerator_fwd.hpp"
#include "../Select.hpp"
#include "../generation/KingJumps.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::king, Jumps, Position>
:
        public std::function<int(Position const&)>
{
private:
        // typedefs

        typedef generator<Color, Material::king, Jumps, Position> KingJumps;

public:
        int operator()(Position const& p) const
        {
                Stack moves;
                moves.reserve(MOVE_RESERVE);
                KingJumps()(p, moves);
                return static_cast<int>(moves.size());
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
