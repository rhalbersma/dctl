#pragma once
#include <dctl/rules/types.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool /* Color */, class /* Material */, class /* Select */, class Range = rules::range::distance_1>
struct detect;

}       // namespace detail
}       // namespace successor
}       // namespace dctl
