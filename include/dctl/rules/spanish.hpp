#pragma once
#include <tuple>        // make_tuple

namespace dctl {
namespace rules {
namespace block_adl {

// http://www.fpdamas.pt/downloads/Regras%20Damas%20Cl%C3%A1ssicas%20-%20atualizado.pdf
struct Spanish
{
        // 2.1 NOMENCLATURA DO TABULEIRO
        static constexpr auto width = 8;                        // 2.1.2
        static constexpr auto height = 8;                       // 2.1.2
        static constexpr auto is_inverted = true;               // 2.1.5

        static constexpr auto is_long_ranged_king = true;
        static constexpr auto is_backward_pawn_jump = false;

        static constexpr auto is_quantity_precedence = true;
        static constexpr auto is_quality_precedence = true;

        struct tuple_type
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return std::make_tuple(
                                a.num_captured_pieces(),
                                a.num_captured_kings()
                        );
                }
        };
};

}       // namespace block_adl

using block_adl::Spanish;

}       // namespace rules
}       // namespace dctl
