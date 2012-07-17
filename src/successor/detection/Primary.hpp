#pragma once
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Rules, typename Board>
struct Driver<Color, Material, Selection, detection, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
        template<typename Position>
        static bool detect(Position const&)
        {
                return false;
        }
};

}       // namespace successor
}       // namespace dctl
