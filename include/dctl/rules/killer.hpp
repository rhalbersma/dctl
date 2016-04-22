#pragma once
#include <dctl/rules/international.hpp> // international

namespace dctl {
namespace rules {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
template<class Parent = international>
struct killer
:
        Parent
{
        static constexpr auto is_halt_behind_king = true;
};

}       // namespace rules
}       // namespace dctl
