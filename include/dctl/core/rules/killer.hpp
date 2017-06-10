#pragma once

namespace dctl::core {
namespace block_adl {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
template<class Parent>
struct killer
:
        Parent
{
        static constexpr auto is_halt_behind_king = true;
};

}       // namespace block_adl

using block_adl::killer;

}       // dctl::core
