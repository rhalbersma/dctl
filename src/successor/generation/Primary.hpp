#pragma once
#include <functional>                   // function
#include "Generator_fwd.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool, int, typename, typename Position>
struct generator
:
        public std::function<void(Position const&, Stack&)>
{
        void operator()(Position const& p, Stack& moves) const
        {
                // no-op
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
