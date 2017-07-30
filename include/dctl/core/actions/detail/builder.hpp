#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // angle, is_orthogonal
#include <dctl/core/board/push_sources.hpp>
#include <dctl/core/board/ray.hpp>
#include <dctl/core/rules/type_traits.hpp>
#include <dctl/core/state/color_piece.hpp>
#include <dctl/util/type_traits.hpp>            // board_t, rules_t, set_t, value_t
#include <algorithm>                            // none_of
#include <cassert>                              // assert
#include <iterator>                             // begin, end, prev
#include <type_traits>                          // is_same

namespace dctl::core {
namespace detail {

template<class Color, class DuplicatesPolicy, class State, class SequenceContainer>
class builder;

template<color Side, class DuplicatesPolicy, class State, class SequenceContainer>
class builder<color_<Side>, DuplicatesPolicy, State, SequenceContainer>
{
public:
        using to_move_ = color_<Side>;
        constexpr static auto to_move_c = color_c<Side>;
        using action_type = value_t<SequenceContainer>;
        using  board_type =       board_t<State>;
        using  rules_type =       rules_t<State>;
        using    set_type =         set_t<State>;

private:
        State const& m_state;
        set_type m_initial_targets;
        set_type m_empty;
        SequenceContainer& m_actions;
        action_type m_candidate_action{};

        template<int Direction>
        using short_push_sources = push_sources<board_type, Direction, short_ranged_tag>;

public:
        builder(State const& s, SequenceContainer& seq)
        :
                m_state{s},
                m_initial_targets(m_state.pieces(!to_move_c)),
                m_empty(m_state.pieces(empty_c)),
                m_actions{seq}
        {}

        auto toggle_king_targets() noexcept
        {
                static_assert(is_superior_rank_jump_v<rules_type>);
                m_initial_targets ^= m_state.pieces(!to_move_c, kings_c);
        }

        auto make_launch(int const sq)
        {
                m_candidate_action.from(sq);
                m_empty.insert(sq);
        }

        auto undo_launch(int const sq)
        {
                m_empty.erase(sq);
        }

        auto capture(int const sq)
        {
                m_candidate_action.capture(sq, is_king(sq));
                if constexpr (is_passing_capture_v<rules_type>) {
                        m_empty.insert(sq);
                }
        }

        auto release(int const sq)
        {
                if constexpr (is_passing_capture_v<rules_type>) {
                        m_empty.erase(sq);
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

        auto finalize(int const sq)
        {
                m_candidate_action.dest(sq);
                precedence_duplicates();
        }

        template<class... Args>
        auto pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
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

        template<int Direction>
        auto targets() const
        {
                return short_push_sources<Direction>{}(targets(), pieces(empty_c));
        }

        template<class Iterator>
        auto has_pawn_target(Iterator it) const
        {
                return ray::has_pawn_jump_target<rules_type>(it, targets<ray::direction_v<Iterator>.value()>());
        }

        template<class Iterator>
        auto is_target(Iterator it) const
        {
                return targets<ray::direction_v<Iterator>.value()>().contains(*it);
        }

        auto not_occupied(int const sq) const
        {
                return m_empty.contains(sq);
        }

        template<int Direction>
        auto path() const
        {
                PP_CONSTEXPR_CONST auto js = board_type::jump_start(angle{Direction});
                return pieces(empty_c) & js;
        }

        template<int Direction>
        auto path(int const sq) const
        {
                return path<Direction>().contains(sq);
        }

        auto is_last_jumped_king(int const sq) const
        {
                return m_state.pieces(kings_c).contains(sq);
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
                return with() == piece::pawns && into() != piece::pawns;
        }

        auto to_move() const noexcept
        {
                return m_state.to_move();
        }

        auto num_captured_pieces() const noexcept
        {
                return 0;
        }

private:
        auto is_king(int sq) const
        {
                return m_state.pieces(kings_c).contains(sq);
        }

        auto precedence_duplicates() const
        {
                if constexpr (
                        is_trivial_precedence_v<rules_type> &&
                        std::is_same_v<DuplicatesPolicy, keep_duplicates_tag>
                ) {
                        assert(m_actions.empty() || precedence::equal_to{}(m_candidate_action, m_actions.back()));
                        m_actions.push_back(m_candidate_action);
                }
                if constexpr (
                        is_trivial_precedence_v<rules_type> &&
                        std::is_same_v<DuplicatesPolicy, drop_duplicates_tag>
                ) {
                        assert(m_actions.empty() || precedence::equal_to{}(m_candidate_action, m_actions.back()));
                        if (m_actions.empty() || is_small() || is_unique()) {
                                m_actions.push_back(m_candidate_action);
                        }
                }
                if constexpr (
                        !is_trivial_precedence_v<rules_type> &&
                        std::is_same_v<DuplicatesPolicy, keep_duplicates_tag>
                ) {
                        if (m_actions.empty() || precedence::equal_to{}(m_candidate_action, m_actions.back())) {
                                return m_actions.push_back(m_candidate_action);
                        }
                        if (precedence::less{}(m_candidate_action, m_actions.back())) {
                                return;
                        }
                        assert(precedence::greater{}(m_candidate_action, m_actions.back()));
                        m_actions.clear();
                        m_actions.push_back(m_candidate_action);
                }
                if constexpr (
                        !is_trivial_precedence_v<rules_type> &&
                        std::is_same_v<DuplicatesPolicy, drop_duplicates_tag>
                ) {
                        if (m_actions.empty()) {
                                return m_actions.push_back(m_candidate_action);
                        }
                        if (precedence::equal_to{}(m_candidate_action, m_actions.back())) {
                                if (is_small() || is_unique()) {
                                        m_actions.push_back(m_candidate_action);
                                }
                                return;
                        }
                        if (precedence::less{}(m_candidate_action, m_actions.back())) {
                                return;
                        }
                        assert(precedence::greater{}(m_candidate_action, m_actions.back()));
                        m_actions.clear();
                        m_actions.push_back(m_candidate_action);
                }
        }

        auto is_small() const noexcept
        {
                return m_candidate_action.num_captured_pieces() < large_jump_v<rules_type>;
        }

        auto is_unique() const // Throws: Nothing.
        {
                static_assert(std::is_same_v<DuplicatesPolicy, drop_duplicates_tag>);
                assert(!m_actions.empty());
                assert(precedence::equal_to{}(m_candidate_action, m_actions.back()));
                return std::none_of(m_actions.cbegin(), m_actions.cend(), [&](auto const& a) { return a == m_candidate_action; });
        }
};

}       // namespace detail
}       // namespace dctl::core
