#pragma once
#include "Enumerator_fwd.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool, int, typename, typename Position>
struct enumerator
{
        int operator()(Position const&)
        {
                return 0;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
