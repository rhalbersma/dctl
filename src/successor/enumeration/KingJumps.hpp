#pragma once
#include "Enumerator_fwd.hpp"
#include "../Select.hpp"
#include "../generation/KingJumps.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::king, select::Jumps, Position>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef generator<Color, Material::king, select::Jumps, Position> KingJumps;

public:
        static int run(Position const& p)
        {
                Stack moves;
                KingJumps::run(p, moves);
                return static_cast<int>(moves.size());
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
