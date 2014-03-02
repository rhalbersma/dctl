#pragma once
#include <dctl/rules/types.hpp>

namespace dctl {
namespace successor {

template<bool /* Color */, class /* Material */, class /* Select */, class Range = rules::range::distance_1>
struct Detect;

}       // namespace successor
}       // namespace dctl
