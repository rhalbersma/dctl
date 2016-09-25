#pragma once
#include <dctl/aima/eval/weight.hpp>    // Weight
#include <dctl/actions.hpp>             // count, select::push
#include <dctl/color_piece.hpp>               // opposite
#include <dctl/mask/column.hpp>         // column
#include <dctl/mask/row.hpp>            // row
#include <xstd/cstddef.hpp>
#include <cstdlib>                      // abs

namespace dctl {
namespace evaluate {

template<class Color>
class Feature
{
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
                        Weight<rules_type, board_type>::material[0] * static_cast<int>(s.num_pieces(Color{}, pawn_type{})) +
                        Weight<rules_type, board_type>::material[1] * static_cast<int>(s.num_pieces(Color{}, king_type{}))
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
                        score += Weight<rules_type, board_type>::tempo[i] * static_cast<int>((s.pieces(Color{}) & mask::row<board_type, Color>{}(i)).count());
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
                                static_cast<int>((s.pieces(Color{}) & mask::column<board_type>{}(         Color {}, i)).count()) +
                                static_cast<int>((s.pieces(Color{}) & mask::column<board_type>{}(opposite<Color>{}, i)).count())
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
                                static_cast<int>((s.pieces(Color{}) & mask::column<board_type>{}(         Color {}, i)).count()) -
                                static_cast<int>((s.pieces(Color{}) & mask::column<board_type>{}(opposite<Color>{}, i)).count())
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
