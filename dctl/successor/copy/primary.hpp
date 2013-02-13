#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)

namespace dctl {
namespace successor {
namespace detail {

// primary template (default implementation)
template
<
        bool /* Color */, typename /* Material */, typename /* Select */
>
struct copy
{
        template<typename Position, typename Vector>
        void operator()(Position const& /* p */, Vector& /* moves */) const
        {
                // no-op
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
