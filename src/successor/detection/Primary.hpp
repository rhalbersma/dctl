#pragma once
#include "Detector_fwd.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, int Material, typename Selection, typename Position>
struct detector<Color, Material, Selection, Position>
:
        // enforce static semantics
        private nonconstructible
{
        static bool run(Position const&)
        {
                return false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
