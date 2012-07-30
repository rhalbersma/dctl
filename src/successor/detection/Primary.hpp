#pragma once
#include "Detector_fwd.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, int Material, typename Selection, typename Position>
struct detector<Color, Material, Selection, Position>
{
        bool operator()(Position const&)
        {
                return false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
