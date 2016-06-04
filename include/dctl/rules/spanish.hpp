#pragma once
#include <tuple>        // make_tuple

namespace dctl {
namespace rules {

// http://www.fpdamas.pt/downloads/Regras%20Damas%20Cl%C3%A1ssicas%20-%20atualizado.pdf
struct spanish
{
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

}       // namespace rules
}       // namespace dctl
