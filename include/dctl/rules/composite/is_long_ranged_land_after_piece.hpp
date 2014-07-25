#pragma once
#include <dctl/rules/irregular/is_directly_land_after_piece.hpp>        // is_directly_land_after_piece
#include <dctl/rules/regular/is_long_ranged_king.hpp>                   // is_long_ranged_king
#include <type_traits>                                                  // integral_constant

namespace dctl {

template<class Rules>
struct is_long_ranged_land_after_piece
:
        std::integral_constant<bool,
                is_long_ranged_king_v<Rules> &&
                !is_directly_land_after_piece_v<Rules>
        >
{};

template<class Rules>
using is_long_ranged_land_after_piece_t = typename is_long_ranged_land_after_piece<Rules>::type;

template<class Rules>
constexpr auto is_long_ranged_land_after_piece_v = is_long_ranged_land_after_piece_t<Rules>::value;

}       // namespace dctl
