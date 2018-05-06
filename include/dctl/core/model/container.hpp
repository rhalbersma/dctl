#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/container/small_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <vector>

namespace dctl::core::model {

template<class Action>
using default_container = boost::container::static_vector<Action, 128>;

}       // namespace dctl::core::model
