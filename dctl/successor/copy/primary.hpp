#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

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
