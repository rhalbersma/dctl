#pragma once
#include <tuple>        // make_tuple

namespace dctl {
namespace rules {

// http://www.fpdamas.pt/downloads/Regras%20Damas%20Cl%C3%A1ssicas%20-%20atualizado.pdf
struct Spanish
{
        static constexpr auto is_long_ranged_king = true;
        static constexpr auto is_backward_pawn_jump = false;

        static constexpr auto is_quantity_precedence = true;
        static constexpr auto is_quality_precedence = true;

        struct precedence_tuple
        {
                template<class Action, class... State>
                constexpr auto operator()(Action const& a, State const&... s) const noexcept
                {
                        return std::make_tuple(
                                a.num_captured(s...),
                                a.num_captured_kings(s...)
                        );
                }
        };
};

}       // namespace rules
}       // namespace dctl
