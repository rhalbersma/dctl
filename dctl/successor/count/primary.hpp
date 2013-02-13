#pragma once
#include <cstddef>                                      // size_t
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)

namespace dctl {
namespace successor {
namespace detail {

template
<
        bool /* Color */, typename /* Material */, typename /* Select */
>
struct count
{
        template<typename Position>
        std::size_t operator()(Position const& /* p */) const
        {
                return 0;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
