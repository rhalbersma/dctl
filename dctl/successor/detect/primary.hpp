#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>

namespace dctl {
namespace successor {
namespace detail {

template
<
        bool /* Color */, typename /* Material */, typename /* Select */, typename /* Range */
>
struct detect
{
        template<typename Position>
        bool operator()(Position const& /* p */) const
        {
                return false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl