#pragma once
#include <functional>                   // function
#include <dctl/successor/detection/Detector_fwd.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, int Material, typename Selection, typename Position, typename Range>
struct detector
:
        public std::function<bool(Position const&)>
{
        bool operator()(Position const&) const
        {
                return false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
