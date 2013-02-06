#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)

namespace dctl {
namespace successor {
namespace detail {

template<bool /* Color */, int /* Material */, typename /* Selection */>
struct count
{
        template<typename Position>
        int operator()(Position const& /* p */) const
        {
                return 0;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
