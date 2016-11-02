#pragma once
#include <dctl/board/angle.hpp>                 // Angle, is_orthogonal
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

template<class Color, class DuplicatesPolicy, class State, class SequenceContainer>
class Builder;

template<Color Side, class DuplicatesPolicy, class State, class SequenceContainer>
class Builder<color_constant<Side>, DuplicatesPolicy, State, SequenceContainer>
{
public:
        using  color_type = color_constant<Side>;
        using action_type = value_t<SequenceContainer>;
        using  board_type =       board_t<State>;
        using  rules_type =       rules_t<State>;
        using    set_type =         set_t<State>;
        using square_type =   std::size_t;

private:
        State const& state;
        set_type initial_targets_;
        set_type none_;
        SequenceContainer& actions;
        action_type candidate_action{};

        template<int Direction>
        using push_sources = board::mask::push_sources<board_type, Direction, short_ranged_tag>;

public:
        explicit Builder(State const& s, SequenceContainer& a)
        :
                state{s},
                initial_targets_(state.pieces(opposite<color_type>{})),
                none_(state.pieces(none_type{})),
                actions{a}
        {}

        auto toggle_king_targets() noexcept
        {
                static_assert(is_superior_rank_jump_v<rules_type>);
                initial_targets_ ^= state.pieces(opposite<color_type>{}, king_type{});
        }

        auto make_launch(std::size_t const sq)
        {
                candidate_action.from(sq);
                none_.set(sq);
        }

        auto undo_launch(std::size_t const sq)
        {
                none_.reset(sq);
        }

        auto capture(std::size_t const sq)
        {
                candidate_action.capture(sq, is_king(sq));
                if constexpr (is_passing_capture_v<rules_type>) {
                        none_.set(sq);
                }
        }

        auto release(std::size_t const sq)
        {
                if constexpr (is_passing_capture_v<rules_type>) {
                        none_.reset(sq);
                }
                candidate_action.release(sq, is_king(sq));
        }

        auto with(Piece const p) noexcept
        {
                candidate_action.with(p);
        }

        auto into(Piece const p) noexcept
        {
                candidate_action.into(p);
        }

        auto finalize(std::size_t const sq)
        {
                candidate_action.dest(sq);
                precedence_duplicates();
        }

        auto active_pawns() const noexcept
        {
                return state.pieces(color_type{}, pawn_type{});
        }

        auto active_kings() const noexcept
        {
                return state.pieces(color_type{}, king_type{});
        }

        auto current_targets() const
        {
                return initial_targets_ & ~candidate_action.captured_pieces();
        }

        template<class Iterator>
        auto current_targets(Iterator it) const
        {
                return current_targets<board::ray::direction_v<Iterator>.degrees()>().test(*it);
        }

        template<int Direction>
        auto current_targets() const
        {
                return push_sources<Direction>{}(current_targets(), pieces(none_type{}));
        }

        auto pieces(none_type) const
        {
                return none_;
        }

        auto not_occupied(square_type const sq) const
        {
                return none_.test(sq);
        }

        template<int Direction>
        auto path() const
        {
                auto constexpr jump_start = board::mask::jump_start<board_type>{}(board::Angle{Direction});
                return pieces(none_type{}) & jump_start;
        }

        template<int Direction>
        auto path(square_type const sq) const
        {
                return path<Direction>().test(sq);
        }

        auto is_last_jumped_king(square_type const sq) const
        {
                return state.pieces(king_type{}).test(sq);
        }

        auto with() const noexcept
        {
                return candidate_action.with();
        }

        auto is_with(Piece const p) const noexcept
        {
                return with() == p;
        }

        auto into() const noexcept
        {
                return candidate_action.into();
        }

        auto is_into(Piece const p) const noexcept
        {
                return into() == p;
        }

        constexpr auto is_promotion() const noexcept
        {
                return is_with(Piece::pawn) && !is_into(Piece::pawn);
        }

        auto to_move() const noexcept
        {
                return state.to_move();
        }

        auto is_to_move(Color const c) const noexcept
        {
                return to_move() == c;
        }

        auto num_captured_pieces() const noexcept
        {
                return 0;
        }

private:
        auto is_king(square_type sq) const
        {
                return state.pieces(king_type{}).test(sq);
        }

        auto precedence_duplicates() const
        {
                if constexpr (
                        !is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, keep_duplicates_tag>{}
                ){
                        assert(actions.empty() || precedence::equal_to{}(candidate_action, actions.back()));
                        actions.push_back(candidate_action);
                }
                if constexpr (
                        !is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, drop_duplicates_tag>{}
                ){
                        assert(actions.empty() || precedence::equal_to{}(candidate_action, actions.back()));
                        if (actions.empty() || is_small() || is_unique())
                                actions.push_back(candidate_action);
                }
                if constexpr (
                        is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, keep_duplicates_tag>{}
                ){
                        if (actions.empty() || precedence::equal_to{}(candidate_action, actions.back()))
                                return actions.push_back(candidate_action);
                        if (precedence::less{}(candidate_action, actions.back()))
                                return;
                        assert(precedence::greater{}(candidate_action, actions.back()));
                        actions.clear();
                        actions.push_back(candidate_action);
                }
                if constexpr (
                        is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, drop_duplicates_tag>{}
                ){
                        if (actions.empty())
                                return actions.push_back(candidate_action);/*
                        if (precedence::equal_to{}(candidate_action, actions.back())) {
                                if (is_small() || is_unique())
                                        actions.push_back(candidate_action);
                                return;
                        }
                        if (precedence::less{}(candidate_action, actions.back()))
                                return;
                        assert(precedence::greater{}(candidate_action, actions.back()));
                        actions.clear();
                        actions.push_back(candidate_action);*/

                        switch(precedence::compare{}(candidate_action, actions.back())) {
                        case -1 :
                                assert(precedence::less{}(candidate_action, actions.back()));
                                return;
                        case  0 :
                                assert(precedence::equal_to{}(candidate_action, actions.back()));
                                if (is_small() || is_unique())
                                        actions.push_back(candidate_action);
                                return;
                        case +1 :
                                assert(precedence::greater{}(candidate_action, actions.back()));
                                actions.clear();
                                actions.push_back(candidate_action);
                                return;
                        default:
                                assert(false);
                        }
                }
        }

        auto is_small() const noexcept
        {
                return candidate_action.num_captured_pieces() < large_jump_v<rules_type>;
        }

        auto is_unique() const // Throws: Nothing.
        {
                static_assert(std::is_same<DuplicatesPolicy, drop_duplicates_tag>{});
                assert(!actions.empty());
                assert(precedence::equal_to{}(candidate_action, actions.back()));
                return boost::algorithm::none_of(actions, [&](auto const& a) { return a == candidate_action; });
        }
};

}       // namespace detail
}       // namespace dctl
