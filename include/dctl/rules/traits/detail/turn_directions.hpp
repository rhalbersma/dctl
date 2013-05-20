#pragma once
#include <boost/mpl/identity.hpp>
#include <dctl/rules/enum.hpp>

namespace dctl {
namespace rules {

// intermediate capture directions
template<typename T>
struct turn_directions
:
        T
{};

template<>
struct turn_directions<directions::orth>
:
        directions::diag
{};

}
}
