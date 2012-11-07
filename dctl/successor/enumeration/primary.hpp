#pragma once
#include <functional>                   // function
#include <dctl/successor/enumeration/enumerator_fwd.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool, int, typename, typename Position>
struct enumerator
:
        public std::function<int(Position const&)>
{
        int operator()(Position const& /* p */) const
        {
                return 0;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
