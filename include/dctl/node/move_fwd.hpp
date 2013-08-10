#pragma once
#include <dctl/utility/int.hpp>

namespace dctl {

template<class>
struct Move_;

using Move = Move_<BitBoard>;

}       // namespace dctl
