#pragma once
#include "Generator_fwd.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool, int, typename, typename Position>
struct generator
{
        void operator()(Position const& p, Stack& moves)
        {
                // no-op
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
