#pragma once
#include <type_traits>                  // integral_constant

namespace dctl {
namespace trait {

template<class Rules>
struct pushsep
:
        std::integral_constant<char, '-'>
{};

}       // namespace trait
}       // namespace dctl
