#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace dctl::core {
namespace block_adl {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
template<class Parent>
struct killer
:
        Parent
{
        constexpr static auto is_halt_behind_king = true;
};

}       // namespace block_adl

using block_adl::killer;

}       // dctl::core
