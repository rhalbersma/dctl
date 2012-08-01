#pragma once
#include "Detector_fwd.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, int Material, typename Selection, typename Position, typename Range>
struct detector<Color, Material, Selection, Position, Range>
{
        bool operator()(Position const&)
        {
                return false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
