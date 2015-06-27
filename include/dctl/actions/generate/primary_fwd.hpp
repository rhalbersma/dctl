#pragma once
#include <dctl/player.hpp>       // Player

namespace dctl {
namespace actions {

template<Player /* ToMove */, class /* Select */, bool /* RemoveDuplicateJumps */, bool /* Reverse */>
class Generate;

}       // namespace actions
}       // namespace dctl
