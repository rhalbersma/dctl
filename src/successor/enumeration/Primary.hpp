#pragma once
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Rules, typename Board>
struct Driver<Color, Material, Selection, enumeration, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
        template<typename Position>
        static int count(Position const&)
        {
                return 0;
        }
};

}       // namespace successor
}       // namespace dctl
