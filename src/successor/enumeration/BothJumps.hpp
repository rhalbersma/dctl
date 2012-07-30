#pragma once
#include "Enumerator_fwd.hpp"
#include "../Select.hpp"
#include "../generation/BothJumps.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct enumerator<Color, Material::both, select::Jumps, Position>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef generator<Color, Material::both, select::Jumps, Position> BothJumps;

public:
        static int run(Position const& p)
        {
                Stack moves;
                BothJumps::run(p, moves);
                return static_cast<int>(moves.size());
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
