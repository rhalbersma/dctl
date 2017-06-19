#pragma once
#include <dctl/core/actions.hpp>             // count, select::push
#include <dctl/core/board/type_traits.hpp>        // file, row
#include <dctl/core/state/color_piece.hpp>         // opposite
#include <dctl/eval/weight.hpp>    // Weight
#include <cstdlib>                      // abs

namespace dctl::eval {

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
                using rules_type = core::rules_t<State>;
                using board_type = core::board_t<State>;
                return
                        Weight<rules_type, board_type>::material[0] * static_cast<int>(s.num_pieces(Color{}, core::pawns_c)) +
                        Weight<rules_type, board_type>::material[1] * static_cast<int>(s.num_pieces(Color{}, core::kings_c))
                ;
        }

        template<class State>
        static auto tempo(State const& s)
        {
                using rules_type = core::rules_t<State>;
                using board_type = core::board_t<State>;
                auto score = 0;
                for (auto i = 1; i < board_type::height; ++i)
                        score += Weight<rules_type, board_type>::tempo[i] * static_cast<int>((s.pieces(Color{}) & core::rank_v<board_type>(Color{}, i)).size());
                return score;
        }

        template<class State>
        static auto center(State const& s)
        {
                using rules_type = core::rules_t<State>;
                using board_type = core::board_t<State>;
                auto score = 0;
                for (auto i = 1; i < board_type::width / 2; ++i) {
                        score += Weight<rules_type, board_type>::center[i] *
                        (
                                static_cast<int>((s.pieces(Color{}) & core::file_v<board_type>( Color{}, i)).size()) +
                                static_cast<int>((s.pieces(Color{}) & core::file_v<board_type>(not Color{}, i)).size())
                        );
                }
                return score;
        }

        template<class State>
        static auto balance(State const& s)
        {
                using rules_type = core::rules_t<State>;
                using board_type = core::board_t<State>;
                auto score = 0;
                for (auto i = 0; i < board_type::width / 2; ++i) {
                        score += Weight<rules_type, board_type>::balance[i] *
                        (
                                static_cast<int>((s.pieces(Color{}) & core::file_v<board_type>( Color{}, i)).size()) -
                                static_cast<int>((s.pieces(Color{}) & core::file_v<board_type>(not Color{}, i)).size())
                        );
                }
                return -abs(score);
        }

        template<class State>
        static auto mobility(State const& s)
        {
                using rules_type = core::rules_t<State>;
                using board_type = core::board_t<State>;
                return Weight<rules_type, board_type>::mobility * static_cast<int>(core::actions<core::select::push>{}.template count<Color>(s));
        }
};

}       // namespace dctl::eval
