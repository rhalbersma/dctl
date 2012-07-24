#pragma once
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../Select.hpp"
#include "../generation/PawnJumps.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
        
// partial specialization for pawn jumps enumeration
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::pawn, select::Jumps, enumeration, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material::pawn, select::Jumps, generation, Rules, Board> PawnJumps;

public:
        template<typename Position>
        static int count(Position const& p)
        {
                Stack moves;
                PawnJumps::generate(p, moves);
                return static_cast<int>(moves.size());
        }
};

}       // namespace successor
}       // namespace dctl
