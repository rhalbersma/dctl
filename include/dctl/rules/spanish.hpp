#pragma once
#include <dctl/piece.hpp>       // king
#include <tuple>                // make_tuple

namespace dctl {
namespace rules {

// http://www.fpdamas.pt/downloads/Regras%20Damas%20Cl%C3%A1ssicas%20-%20atualizado.pdf

struct Spanish
{
        static constexpr auto is_long_ranged_king = true;
        static constexpr auto is_backward_pawn_jump = false;

        static constexpr auto is_quantity = true;
        static constexpr auto is_quality = true;

        struct precedence
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return std::make_tuple(
                                a.num_captured(), a.num_captured(Piece::king)
                        );
                }
        };
};

}       // namespace rules
}       // namespace dctl
