#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>  // copy (primary template)
#include <dctl/node/move.hpp>                   // Move
#include <dctl/node/stack.hpp>                  // Vector

namespace dctl {
namespace successor {
namespace detail {

// primary template (default implementation)
template<bool /* Color */, int /* Material */, typename /* Selection */, typename Position>
struct copy
{
        void operator()(Position const& /* p */, Vector<Move>& /* m */) const
        {
                // no-op
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
