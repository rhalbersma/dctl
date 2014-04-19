#pragma once
#include <type_traits>                  // integral_constant

namespace dctl {
namespace move {
namespace traits {

template<class Rules>
struct pushsep
:
        std::integral_constant<char, '-'>
{};

}       // namespace traits
}       // namespace move
}       // namespace dctl
