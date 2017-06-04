#pragma once

namespace dctl {
namespace core {
namespace block_adl {

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct russian
{
        static constexpr auto is_long_ranged_king = true;       // 1.4.5

        static constexpr auto is_backward_pawn_jump = true;     // 1.5.3
        static constexpr auto is_passing_promotion = true;      // 1.5.15

        static constexpr auto is_algebraic_notation = true;
        static constexpr auto jumpsep = ':';
};

}       // namespace block_adl

using block_adl::russian;

using shashki = russian;

}       // namespace core
}       // namespace dctl
