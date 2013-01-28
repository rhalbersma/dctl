#pragma once
#include <dctl/successor/count/enumerator_fwd.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool /* Color */, int /* Material */, typename /* Selection */, typename Position>
struct enumerator
{
        int operator()(Position const& /* p */) const
        {
                return 0;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
