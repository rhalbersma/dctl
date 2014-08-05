#pragma once
#include <dctl/rules/international/rules.hpp>   // Rules
#include <dctl/rules/killer/value.hpp>          // Value

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

}       // namespace rules
}       // namespace dctl
