#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>
#include <dctl/core/state/color.hpp>
#include <dctl/core/state/piece.hpp>
#include <dctl/util/type_traits.hpp>            // board_t, rules_t, set_t, value_t
#include <algorithm>                            // none_of
#include <cassert>                              // assert
#include <concepts>                             // same_as
#include <iterator>                             // begin, end, prev

namespace dctl::core::model {
namespace detail {

template<class...>
class builder;

template<color Side, class DuplicatesPolicy, class State, class SequenceContainer>
class builder<color_<Side>, DuplicatesPolicy, State, SequenceContainer>
{
public:
        using to_move_ = color_<Side>;
        constexpr static auto to_move_c = color_c<Side>;
        using action_type = value_t<SequenceContainer>;
        using  board_type = board_t<State>;
        using  rules_type = rules_t<State>;
        using   mask_type =  mask_t<State>;
        using    set_type =   set_t<mask_type>;

private:
        State const& m_state;
        set_type m_initial_targets;
        set_type m_empty;
        SequenceContainer& m_actions;
        action_type m_candidate_action{};

public:
        builder(State const& s, SequenceContainer& seq)
        :
                m_state{s},
                m_initial_targets(m_state.pieces(!to_move_c)),
                m_empty(m_state.pieces(empty_c)),
                m_actions{seq}
        {}

        auto toggle_king_targets() noexcept
                requires is_superior_rank_jump_v<rules_type>
        {
                m_initial_targets ^= m_state.pieces(!to_move_c, king_c);
        }

        auto lift(int const sq)
        {
                m_candidate_action.from(sq);
                m_empty.add(sq);
        }

        auto drop(int const sq)
        {
                m_empty.pop(sq);
        }

        auto capture(int const sq)
        {
                m_candidate_action.capture(sq, is_king(sq));
                if constexpr (is_passing_capture_v<rules_type>) {
                        m_empty.add(sq);
                }
        }

        auto release(int const sq)
        {
                if constexpr (is_passing_capture_v<rules_type>) {
                        m_empty.pop(sq);
                }
                m_candidate_action.release(sq, is_king(sq));
        }

        auto with(piece const p) noexcept
        {
                m_candidate_action.with(p);
        }

        auto into(piece const p) noexcept
        {
                m_candidate_action.into(p);
        }

        auto finalize(int const sq, piece const p)
        {
                m_candidate_action.into(p);
                m_candidate_action.dest(sq);
                precedence_duplicates();
        }

        auto finalize(int const sq)
        {
                assert(into() == piece::king);
                m_candidate_action.dest(sq);
                precedence_duplicates();
        }

        template<class... Args>
                requires (sizeof...(Args) <= 2)
        auto pieces(Args&&... args) const noexcept
        {
                return m_state.pieces(std::forward<Args>(args)...);
        }

        auto pieces(empty_) const noexcept
        {
                return m_empty;
        }

        auto pieces(occup_) const noexcept
        {
                return board_type::squares ^ m_empty;
        }

        auto targets() const noexcept
        {
                return m_initial_targets - m_candidate_action.captured_pieces();
        }

        auto is_last_jumped_king(int const sq) const
        {
                return m_state.pieces(king_c).contains(sq);
        }

        auto with() const noexcept
        {
                return m_candidate_action.with();
        }

        auto into() const noexcept
        {
                return m_candidate_action.into();
        }

        constexpr auto is_promotion() const noexcept
        {
                return with() == piece::pawn && into() != piece::pawn;
        }

        auto to_move() const noexcept
        {
                return m_state.to_move();
        }

private:
        auto is_king(int sq) const
        {
                return m_state.pieces(king_c).contains(sq);
        }

        auto precedence_duplicates() const
        {
                if constexpr (
                        is_trivial_precedence_v<rules_type> &&
                        std::same_as<DuplicatesPolicy, keep_duplicates_tag>
                ) {
                        assert(m_actions.empty() || (capture_precedence(m_candidate_action) <=> capture_precedence(m_actions.back())) == 0);
                        m_actions.push_back(m_candidate_action);
                }
                if constexpr (
                        is_trivial_precedence_v<rules_type> &&
                        std::same_as<DuplicatesPolicy, drop_duplicates_tag>
                ) {
                        assert(m_actions.empty() || (capture_precedence(m_candidate_action) <=> capture_precedence(m_actions.back())) == 0);
                        if (m_actions.empty() || is_small() || is_unique()) {
                                m_actions.push_back(m_candidate_action);
                        }
                }
                if constexpr (
                        !is_trivial_precedence_v<rules_type> &&
                        std::same_as<DuplicatesPolicy, keep_duplicates_tag>
                ) {
                        if (m_actions.empty()) {
                                return m_actions.push_back(m_candidate_action);
                        }
                        auto const order = capture_precedence(m_candidate_action) <=> capture_precedence(m_actions.back());
                        if (order >= 0) {
                                if (order > 0) {
                                        m_actions.clear();
                                }
                                m_actions.push_back(m_candidate_action);
                        }
                }
                if constexpr (
                        !is_trivial_precedence_v<rules_type> &&
                        std::same_as<DuplicatesPolicy, drop_duplicates_tag>
                ) {
                        if (m_actions.empty()) {
                                return m_actions.push_back(m_candidate_action);
                        }
                        auto const order = capture_precedence(m_candidate_action) <=> capture_precedence(m_actions.back());
                        if (order >= 0) {
                                if (order > 0) {
                                        m_actions.clear();
                                        m_actions.push_back(m_candidate_action);
                                } else if (is_small() || is_unique()) {
                                        m_actions.push_back(m_candidate_action);
                                }
                        }
                }
        }

        auto is_small() const noexcept
        {
                return m_candidate_action.num_captured_pieces() < large_jump_v<rules_type>;
        }

        auto is_unique() const noexcept
                requires std::same_as<DuplicatesPolicy, drop_duplicates_tag>
        {
                assert(!m_actions.empty());
                assert((capture_precedence(m_candidate_action) <=> capture_precedence(m_actions.back())) == 0);
                return std::ranges::none_of(m_actions, [&](auto const& a) { return a == m_candidate_action; });
        }
};

}       // namespace detail
}       // namespace dctl::core::model
