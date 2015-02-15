#pragma once
#include <dctl/rules/international/rules.hpp>   // Rules

namespace dctl {
namespace killer {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules

template<class Parent = international::Rules>
struct Rules
:
        // required
        Parent
{
        // optional
        static constexpr auto is_directly_halt_after_final_king = true;
};

}       // namespace killer
}       // namespace dctl
