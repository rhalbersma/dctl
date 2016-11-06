#pragma once
#include <dctl/board/angle.hpp>                 // angle, is_orthogonal
#include <dctl/board/mask/push_sources.hpp>
#include <dctl/board/mask/jump_start.hpp>       // jump_start
#include <dctl/board/ray.hpp>
#include <dctl/color_piece.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/utility/type_traits.hpp>         // board_t, rules_t, set_t, value_t
#include <boost/algorithm/cxx11/none_of.hpp>    // none_of
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <iterator>                             // begin, end, prev
#include <type_traits>                          // is_same

namespace dctl {
namespace detail {

template<class color, class DuplicatesPolicy, class State, class SequenceContainer>
class builder;

template<color Side, class DuplicatesPolicy, class State, class SequenceContainer>
class builder<color_<Side>, DuplicatesPolicy, State, SequenceContainer>
{
public:
        using to_move_ = color_<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        using action_type = value_t<SequenceContainer>;
        using  board_type =       board_t<State>;
        using  rules_type =       rules_t<State>;
        using    set_type =         set_t<State>;
        using square_type =   std::size_t;

private:
        State const& m_state;
        set_type m_initial_targets;
        set_type m_empty;
        SequenceContainer& m_actions;
        action_type m_candidate_action{};

        template<int Direction>
        using push_sources = board::mask::push_sources<board_type, Direction, short_ranged_tag>;

public:
        builder(State const& s, SequenceContainer& a)
        :
                m_state{s},
                m_initial_targets(m_state.pieces(!to_move_c)),
                m_empty(m_state.pieces(empty_c)),
                m_actions{a}
        {}

        auto toggle_king_targets() noexcept
        {
                static_assert(is_superior_rank_jump_v<rules_type>);
                m_initial_targets ^= m_state.pieces(!to_move_c, kings_c);
        }

        auto make_launch(std::size_t const sq)
        {
                m_candidate_action.from(sq);
                m_empty.set(sq);
        }

        auto undo_launch(std::size_t const sq)
        {
                m_empty.reset(sq);
        }

        auto capture(std::size_t const sq)
        {
                m_candidate_action.capture(sq, is_king(sq));
                if constexpr (is_passing_capture_v<rules_type>) {
                        m_empty.set(sq);
                }
        }

        auto release(std::size_t const sq)
        {
                if constexpr (is_passing_capture_v<rules_type>) {
                        m_empty.reset(sq);
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

        auto finalize(std::size_t const sq)
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

        auto targets() const noexcept
        {
                return m_initial_targets & ~m_candidate_action.captured_pieces();
        }

        template<int Direction>
        auto targets() const
        {
                return push_sources<Direction>{}(targets(), pieces(empty_c));
        }

        template<class Iterator>
        auto is_target(Iterator it) const
        {
                return targets<board::ray::direction_v<Iterator>.value()>().test(*it);
        }

        auto not_occupied(square_type const sq) const
        {
                return m_empty.test(sq);
        }

        template<int Direction>
        auto path() const
        {
                auto constexpr jump_start = board::mask::jump_start<board_type>{}(board::angle{Direction});
                return pieces(empty_c) & jump_start;
        }

        template<int Direction>
        auto path(square_type const sq) const
        {
                return path<Direction>().test(sq);
        }

        auto is_last_jumped_king(square_type const sq) const
        {
                return m_state.pieces(kings_c).test(sq);
        }

        auto with() const noexcept
        {
                return m_candidate_action.with();
        }

        auto is_with(piece const p) const noexcept
        {
                return with() == p;
        }

        auto into() const noexcept
        {
                return m_candidate_action.into();
        }

        auto is_into(piece const p) const noexcept
        {
                return into() == p;
        }

        constexpr auto is_promotion() const noexcept
        {
                return is_with(piece::pawn) && !is_into(piece::pawn);
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
        auto is_king(square_type sq) const
        {
                return m_state.pieces(kings_c).test(sq);
        }

        auto precedence_duplicates() const
        {
                if constexpr (
                        !is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, keep_duplicates_tag>{}
                ){
                        assert(m_actions.empty() || precedence::equal_to{}(m_candidate_action, m_actions.back()));
                        m_actions.push_back(m_candidate_action);
                }
                if constexpr (
                        !is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, drop_duplicates_tag>{}
                ){
                        assert(m_actions.empty() || precedence::equal_to{}(m_candidate_action, m_actions.back()));
                        if (m_actions.empty() || is_small() || is_unique())
                                m_actions.push_back(m_candidate_action);
                }
                if constexpr (
                        is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, keep_duplicates_tag>{}
                ){
                        if (m_actions.empty() || precedence::equal_to{}(m_candidate_action, m_actions.back()))
                                return m_actions.push_back(m_candidate_action);
                        if (precedence::less{}(m_candidate_action, m_actions.back()))
                                return;
                        assert(precedence::greater{}(m_candidate_action, m_actions.back()));
                        m_actions.clear();
                        m_actions.push_back(m_candidate_action);
                }
                if constexpr (
                        is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, drop_duplicates_tag>{}
                ){
                        if (m_actions.empty())
                                return m_actions.push_back(m_candidate_action);/*
                        if (precedence::equal_to{}(m_candidate_action, m_actions.back())) {
                                if (is_small() || is_unique())
                                        m_actions.push_back(m_candidate_action);
                                return;
                        }
                        if (precedence::less{}(m_candidate_action, m_actions.back()))
                                return;
                        assert(precedence::greater{}(m_candidate_action, m_actions.back()));
                        m_actions.clear();
                        m_actions.push_back(m_candidate_action);*/

                        switch(precedence::compare{}(m_candidate_action, m_actions.back())) {
                        case -1 :
                                assert(precedence::less{}(m_candidate_action, m_actions.back()));
                                return;
                        case  0 :
                                assert(precedence::equal_to{}(m_candidate_action, m_actions.back()));
                                if (is_small() || is_unique())
                                        m_actions.push_back(m_candidate_action);
                                return;
                        case +1 :
                                assert(precedence::greater{}(m_candidate_action, m_actions.back()));
                                m_actions.clear();
                                m_actions.push_back(m_candidate_action);
                                return;
                        default:
                                assert(false);
                        }
                }
        }

        auto is_small() const noexcept
        {
                return m_candidate_action.num_captured_pieces() < large_jump_v<rules_type>;
        }

        auto is_unique() const // Throws: Nothing.
        {
                static_assert(std::is_same<DuplicatesPolicy, drop_duplicates_tag>{});
                assert(!m_actions.empty());
                assert(precedence::equal_to{}(m_candidate_action, m_actions.back()));
                return boost::algorithm::none_of(m_actions, [&](auto const& a) { return a == m_candidate_action; });
        }
};

}       // namespace detail
}       // namespace dctl
