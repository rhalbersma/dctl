#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/model.hpp>  // count, select::move
#include <dctl/eval/weight.hpp> // Weight
#include <cstdlib>              // abs
#include <type_traits>          // remove_cvref_t

namespace dctl::eval {

template<class Color>
class Feature
{
public:
        static auto value(auto const& state)
        {
                auto score = 0;
                score += material(state);
                score += tempo(state);
                score += center(state);
                score += balance(state);
                score += mobility(state);
                return score;
        }

        static auto material(auto const& state)
        {
                using state_type = std::remove_cvref_t<decltype(state)>;
                using rules_type = core::rules_t<state_type>;
                using board_type = core::board_t<state_type>;
                return
                        Weight<rules_type, board_type>::material[0] * state.num_pieces(Color{}, core::pawn_c) +
                        Weight<rules_type, board_type>::material[1] * state.num_pieces(Color{}, core::king_c)
                ;
        }

        static auto tempo(auto const& state)
        {
                using state_type = std::remove_cvref_t<decltype(state)>;
                using rules_type = core::rules_t<state_type>;
                using board_type = core::board_t<state_type>;
                using  mask_type = core:: mask_t<state_type>;
                auto score = 0;
                for (auto i = 1; i < board_type::height; ++i) {
                        score += Weight<rules_type, board_type>::tempo[i] * static_cast<int>((state.pieces(Color{}) & mask_type::rank(Color{}, i)).size());
                }
                return score;
        }

        static auto center(auto const& state)
        {
                using state_type = std::remove_cvref_t<decltype(state)>;
                using rules_type = core::rules_t<state_type>;
                using board_type = core::board_t<state_type>;
                using  mask_type = core:: mask_t<state_type>;
                auto score = 0;
                for (auto i = 1; i < board_type::width / 2; ++i) {
                        score += Weight<rules_type, board_type>::center[i] *
                        (
                                static_cast<int>((state.pieces(Color{}) & mask_type::file( Color{}, i)).size()) +
                                static_cast<int>((state.pieces(Color{}) & mask_type::file(!Color{}, i)).size())
                        );
                }
                return score;
        }

        static auto balance(auto const& state)
        {
                using state_type = std::remove_cvref_t<decltype(state)>;
                using rules_type = core::rules_t<state_type>;
                using board_type = core::board_t<state_type>;
                using  mask_type = core:: mask_t<state_type>;
                auto score = 0;
                for (auto i = 0; i < board_type::width / 2; ++i) {
                        score += Weight<rules_type, board_type>::balance[i] *
                        (
                                static_cast<int>((state.pieces(Color{}) & mask_type::file( Color{}, i)).size()) -
                                static_cast<int>((state.pieces(Color{}) & mask_type::file(!Color{}, i)).size())
                        );
                }
                return -abs(score);
        }

        static auto mobility(auto const& state)
        {
                using state_type = std::remove_cvref_t<decltype(state)>;
                using rules_type = core::rules_t<state_type>;
                using board_type = core::board_t<state_type>;
                return Weight<rules_type, board_type>::mobility * core::drop_duplicates_gen.template count<Color>(state);
        }
};

}       // namespace dctl::eval
