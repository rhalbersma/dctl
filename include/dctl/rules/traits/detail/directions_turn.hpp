#pragma once
#include <boost/mpl/identity.hpp>
#include <dctl/rules/enum.hpp>

namespace dctl {
namespace rules {

// intermediate capture directions
template<class T>
struct directions_turn
:
        T
{};

template<>
struct directions_turn<directions::orth>
:
        directions::diag
{};

}
}
