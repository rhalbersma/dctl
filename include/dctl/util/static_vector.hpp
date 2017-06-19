#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/container/static_vector.hpp>

namespace dctl {

#define DCTL_PP_STACK_RESERVE 64

template<class T>
using static_vector = boost::container::static_vector<T, DCTL_PP_STACK_RESERVE>;

using Variation = static_vector<int>;
using Order = static_vector<int>;

}       // namespace dctl
