#pragma once
#include <dctl/aima/eval/weight.hpp>    // Weight
#include <dctl/actions.hpp>             // count, select::push
#include <dctl/board/mask/column.hpp>   // column
#include <dctl/board/mask/row.hpp>      // row
#include <dctl/color_piece.hpp>         // opposite
#include <xstd/cstddef.hpp>
#include <cstdlib>                      // abs

namespace dctl {
namespace evaluate {

template<class Color>
class Feature
{
        static constexpr auto to_move_c = Color{};
public:
        template<class State>
        static auto value(State const& s)
        {
                auto score = 0;
                score += material(s);
                score += tempo(s);
                score += center(s);
                score += balance(s);
                score += mobility(s);
                return score;
        }

        template<class State>
        static auto material(State const& s)
        {
                using rules_type = rules_t<State>;
                using board_type = board_t<State>;
                return
                        Weight<rules_type, board_type>::material[0] * static_cast<int>(s.num_pieces(to_move_c, pawn_c)) +
                        Weight<rules_type, board_type>::material[1] * static_cast<int>(s.num_pieces(to_move_c, king_c))
                ;
        }

        template<class State>
        static auto tempo(State const& s)
        {
                using rules_type = rules_t<State>;
                using board_type = board_t<State>;
                using namespace xstd::support_literals;
                auto score = 0;
                for (auto i = 1_zu; i < board_type::height; ++i)
                        score += Weight<rules_type, board_type>::tempo[i] * static_cast<int>((s.pieces(to_move_c) & board::mask::row<board_type, Color>{}(i)).count());
                return score;
        }

        template<class State>
        static auto center(State const& s)
        {
                using rules_type = rules_t<State>;
                using board_type = board_t<State>;
                using namespace xstd::support_literals;
                auto score = 0;
                for (auto i = 1_zu; i < board_type::width / 2; ++i) {
                        score += Weight<rules_type, board_type>::center[i] *
                        (
                                static_cast<int>((s.pieces(to_move_c) & board::mask::column<board_type>{}( to_move_c, i)).count()) +
                                static_cast<int>((s.pieces(to_move_c) & board::mask::column<board_type>{}(!to_move_c, i)).count())
                        );
                }
                return score;
        }

        template<class State>
        static auto balance(State const& s)
        {
                using rules_type = rules_t<State>;
                using board_type = board_t<State>;
                using namespace xstd::support_literals;
                auto score = 0;
                for (auto i = 0_zu; i < board_type::width / 2; ++i) {
                        score += Weight<rules_type, board_type>::balance[i] *
                        (
                                static_cast<int>((s.pieces(to_move_c) & board::mask::column<board_type>{}( to_move_c, i)).count()) -
                                static_cast<int>((s.pieces(to_move_c) & board::mask::column<board_type>{}(!to_move_c, i)).count())
                        );
                }
                return -abs(score);
        }

        template<class State>
        static auto mobility(State const& s)
        {
                using rules_type = rules_t<State>;
                using board_type = board_t<State>;
                return Weight<rules_type, board_type>::mobility * static_cast<int>(Actions<select::push>{}.template count<Color>(s));
        }
};

}       // namespace evaluate
}       // namespace dctl
