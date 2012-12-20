#pragma once
#include <dctl/successor/detection/detector_fwd.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, int Material, typename Selection, typename Position, typename Range>
struct detector
{
        bool operator()(Position const& /* p */) const
        {
                return false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
