#pragma once
#include <boost/container/static_vector.hpp>

namespace dctl {

#define DCTL_PP_STACK_RESERVE 64

template<class T>
using static_vector = boost::container::static_vector<T, DCTL_PP_STACK_RESERVE>;

using Variation = static_vector<int>;
using Order = static_vector<int>;

}       // namespace dctl
