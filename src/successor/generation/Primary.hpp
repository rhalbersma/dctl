#pragma once
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Rules, typename Board>
struct Driver<Color, Material, Selection, generation, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
        template<typename Position>
        static void generate(Position const&, Stack&)
        {
                // no-op
        }
};

}       // namespace successor
}       // namespace dctl
