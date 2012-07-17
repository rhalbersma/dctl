#pragma once
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../Selection.hpp"
#include "../generation/BothJumps.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::both, select::Jumps, enumeration, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material::both, select::Jumps, generation, Rules, Board> BothJumps;

public:
        template<typename Position>
        static int count(Position const& p)
        {
                Stack moves;
                BothJumps::generate(p, moves);
                return static_cast<int>(moves.size());
        }
};

}       // namespace successor
}       // namespace dctl
