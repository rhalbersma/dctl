#pragma once
#include <dctl/core/rules/international.hpp> // international

namespace dctl {
namespace rules {
namespace block_adl {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
template<class Parent = international>
struct killer
:
        Parent
{
        static constexpr auto is_halt_behind_king = true;
};

}       // namespace block_adl

using block_adl::killer;

}       // namespace rules
}       // namespace dctl
