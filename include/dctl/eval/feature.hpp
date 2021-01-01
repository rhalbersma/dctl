#pragma once

//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/model.hpp>          // count, select::move
#include <dctl/eval/weight.hpp>         // Weight
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
                        Weight<rules_type, board_type>::material[0] * static_cast<int>(s.num_pieces(Color{}, core::pawn_c)) +
                        Weight<rules_type, board_type>::material[1] * static_cast<int>(s.num_pieces(Color{}, core::king_c))
                ;
        }

        template<class State>
        static auto tempo(State const& s)
        {
                using rules_type = core::rules_t<State>;
                using board_type = core::board_t<State>;
                using  mask_type = core:: mask_t<State>;
                auto score = 0;
                for (auto i = 1; i < board_type::height; ++i) {
                        score += Weight<rules_type, board_type>::tempo[i] * static_cast<int>((s.pieces(Color{}) & mask_type::rank(Color{}, i)).ssize());
                }
                return score;
        }

        template<class State>
        static auto center(State const& s)
        {
                using rules_type = core::rules_t<State>;
                using board_type = core::board_t<State>;
                using  mask_type = core:: mask_t<State>;
                auto score = 0;
                for (auto i = 1; i < board_type::width / 2; ++i) {
                        score += Weight<rules_type, board_type>::center[i] *
                        (
                                static_cast<int>((s.pieces(Color{}) & mask_type::file( Color{}, i)).ssize()) +
                                static_cast<int>((s.pieces(Color{}) & mask_type::file(!Color{}, i)).ssize())
                        );
                }
                return score;
        }

        template<class State>
        static auto balance(State const& s)
        {
                using rules_type = core::rules_t<State>;
                using board_type = core::board_t<State>;
                using  mask_type = core:: mask_t<State>;
                auto score = 0;
                for (auto i = 0; i < board_type::width / 2; ++i) {
                        score += Weight<rules_type, board_type>::balance[i] *
                        (
                                static_cast<int>((s.pieces(Color{}) & mask_type::file( Color{}, i)).ssize()) -
                                static_cast<int>((s.pieces(Color{}) & mask_type::file(!Color{}, i)).ssize())
                        );
                }
                return -abs(score);
        }

        template<class State>
        static auto mobility(State const& s)
        {
                using rules_type = core::rules_t<State>;
                using board_type = core::board_t<State>;
                return Weight<rules_type, board_type>::mobility * static_cast<int>(core::model::actions{}.template count<Color>(s));
        }
};

}       // namespace dctl::eval
