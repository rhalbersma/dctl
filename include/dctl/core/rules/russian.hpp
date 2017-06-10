#pragma once

namespace dctl::core {
namespace block_adl {

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct russian
{
        constexpr static auto is_long_ranged_king = true;       // 1.4.5

        constexpr static auto is_backward_pawn_jump = true;     // 1.5.3
        constexpr static auto is_passing_promotion = true;      // 1.5.15

        constexpr static auto is_algebraic_notation = true;
        constexpr static auto jumpsep = ':';
};

}       // namespace block_adl

using block_adl::russian;

using shashki = russian;

}       // namespace dctl::core
