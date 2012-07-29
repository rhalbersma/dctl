#pragma once
#include "Select.hpp"
#include "../node/Material.hpp"

namespace dctl {
namespace successor {

// primary template
template<bool, int, typename, typename, typename, typename> struct Driver;
template<bool, int, typename, typename> struct generator;

}       // namespace successor
}       // namespace dctl
