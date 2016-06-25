#pragma once
#include <dctl/board/angle.hpp>                 // angle, is_orthogonal
#include <dctl/board/ray.hpp>
#include <dctl/mask/push_sources.hpp>
#include <dctl/color.hpp>
#include <dctl/mask/jump_start.hpp>             // jump_start
#include <dctl/piece.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/state/pieces.hpp>
#include <dctl/utility/type_traits.hpp>         // board_t, rules_t, set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type, value_t
#include <boost/algorithm/cxx11/none_of.hpp>    // none_of
#include <experimental/type_traits>
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <iterator>                             // begin, end, prev

namespace dctl {
namespace core {
namespace detail {

template<color ToMove, class DuplicatesPolicy, class State, class SequenceContainer>
class Builder
{
public:
        using action_type = xstd::value_t<SequenceContainer>;
        using  board_type = board_t<State>;
        using  rules_type = rules_t<State>;
        using    set_type =   set_t<State>;
        using square_type = std::size_t;

private:
        State const& state;
        set_type initial_targets_;
        set_type not_occupied_;
        SequenceContainer& actions;
        action_type candidate_action{};

        template<int Direction>
        using push_sources = mask::push_sources<board_type, Direction, short_ranged_tag>;

public:
        explicit Builder(State const& s, SequenceContainer& a)
        :
                state{s},
                initial_targets_(pieces<!ToMove>(state)),
                not_occupied_(state.not_occupied()),
                actions{a}
        {}

        auto toggle_king_targets() noexcept
        {
                static_assert(is_superior_rank_jump_or_v<rules_type>);
                initial_targets_ ^= pieces<!ToMove, piece::king>(state);
        }

        auto make_launch(std::size_t const sq)
        {
                candidate_action.from(sq);
                not_occupied_.set(sq);
        }

        auto undo_launch(std::size_t const sq)
        {
                not_occupied_.reset(sq);
        }

        auto capture(std::size_t const sq)
        {
                capture_dispatch(sq, capture_category_t<rules_type>{});
        }

        auto release(std::size_t const sq)
        {
                release_dispatch(sq, capture_category_t<rules_type>{});
        }

        auto with(piece const p) noexcept
        {
                candidate_action.with(p);
        }

        auto into(piece const p) noexcept
        {
                candidate_action.into(p);
        }

        auto finalize(std::size_t const sq)
        {
                candidate_action.dest(sq);
                precedence_duplicates_dispatch(precedence_category_t<rules_type>{}, DuplicatesPolicy{});
        }

        auto active_pawns() const noexcept
        {
                return pieces<ToMove, piece::pawn>(state);
        }

        auto active_kings() const noexcept
        {
                return pieces<ToMove, piece::king>(state);
        }

        auto current_targets() const
        {
                return initial_targets_ & ~candidate_action.captured_pieces();
        }

        template<class Iterator>
        auto current_targets(Iterator it) const
        {
                return current_targets<board::ray::direction_v<Iterator>.degrees>().test(*it);
        }

        template<int Direction>
        auto current_targets() const
        {
                return push_sources<Direction>{}(current_targets(), not_occupied());
        }

        auto not_occupied() const
        {
                return not_occupied_;
        }

        auto not_occupied(square_type const sq) const
        {
                return not_occupied_.test(sq);
        }

        template<int Direction>
        auto path() const
        {
                auto constexpr jump_start = mask::jump_start<board_type>{}(angle{Direction});
                return not_occupied() & jump_start;
        }

        template<int Direction>
        auto path(square_type const sq) const
        {
                return path<Direction>().test(sq);
        }

        auto is_last_jumped_king(square_type const sq) const
        {
                return pieces<piece::king>(state).test(sq);
        }

        auto with() const noexcept
        {
                return candidate_action.with();
        }

        auto is_with(piece const p) const noexcept
        {
                return with() == p;
        }

        auto into() const noexcept
        {
                return candidate_action.into();
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
                return state.to_move();
        }

        auto is_to_move(color const c) const noexcept
        {
                return to_move() == c;
        }

        auto num_captured_pieces() const noexcept
        {
                return 0;
        }

private:
        auto capture_dispatch(std::size_t const sq, stopped_capture_tag)
        {
                candidate_action.capture(sq, is_king(sq));
        }

        auto capture_dispatch(std::size_t const sq, passing_capture_tag)
        {
                candidate_action.capture(sq, is_king(sq));
                not_occupied_.set(sq);
        }

        auto release_dispatch(std::size_t const sq, stopped_capture_tag)
        {
                candidate_action.release(sq, is_king(sq));
        }

        auto release_dispatch(std::size_t const sq, passing_capture_tag)
        {
                candidate_action.release(sq, is_king(sq));
                not_occupied_.reset(sq);
        }

        auto is_king(square_type sq) const
        {
                return pieces<piece::king>(state).test(sq);
        }

        auto precedence_duplicates_dispatch(trivial_precedence_tag, keep_duplicates_tag) const
        {
                assert(actions.empty() || precedence::equal_to{}(candidate_action, actions.back()));
                actions.push_back(candidate_action);
        }

        auto precedence_duplicates_dispatch(trivial_precedence_tag, drop_duplicates_tag) const
        {
                assert(actions.empty() || precedence::equal_to{}(candidate_action, actions.back()));
                if (actions.empty() || is_small() || is_unique())
                        actions.push_back(candidate_action);
        }

        auto precedence_duplicates_dispatch(nontrivial_precedence_tag, keep_duplicates_tag) const
        {
                if (actions.empty() || precedence::equal_to{}(candidate_action, actions.back()))
                        return actions.push_back(candidate_action);
                if (precedence::less{}(candidate_action, actions.back()))
                        return;
                assert(precedence::greater{}(candidate_action, actions.back()));
                actions.clear();
                actions.push_back(candidate_action);
        }

        auto precedence_duplicates_dispatch(nontrivial_precedence_tag, drop_duplicates_tag) const
        {
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

        auto is_small() const noexcept
        {
                return candidate_action.num_captured_pieces() < large_jump_v<rules_type>;
        }

        auto is_unique() const // Throws: Nothing.
        {
                static_assert(std::experimental::is_same_v<DuplicatesPolicy, drop_duplicates_tag>);
                assert(!actions.empty());
                assert(precedence::equal_to{}(candidate_action, actions.back()));
                return boost::algorithm::none_of(actions, [&](auto const& a) { return a == candidate_action; });
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
