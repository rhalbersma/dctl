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
        using to_move_ = color_constant<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        using action_type = value_t<SequenceContainer>;
        using  board_type =       board_t<State>;
        using  rules_type =       rules_t<State>;
        using    set_type =         set_t<State>;
        using square_type =   std::size_t;

private:
        State const& state_;
        set_type initial_targets_;
        set_type empty_;
        SequenceContainer& actions_;
        action_type candidate_action_{};

        template<int Direction>
        using push_sources = board::mask::push_sources<board_type, Direction, short_ranged_tag>;

public:
        Builder(State const& s, SequenceContainer& a)
        :
                state_{s},
                initial_targets_(state_.pieces(!to_move_c)),
                empty_(state_.pieces(none_c)),
                actions_{a}
        {}

        auto toggle_king_targets() noexcept
        {
                static_assert(is_superior_rank_jump_v<rules_type>);
                initial_targets_ ^= state_.pieces(!to_move_c, king_c);
        }

        auto make_launch(std::size_t const sq)
        {
                candidate_action_.from(sq);
                empty_.set(sq);
        }

        auto undo_launch(std::size_t const sq)
        {
                empty_.reset(sq);
        }

        auto capture(std::size_t const sq)
        {
                candidate_action_.capture(sq, is_king(sq));
                if constexpr (is_passing_capture_v<rules_type>) {
                        empty_.set(sq);
                }
        }

        auto release(std::size_t const sq)
        {
                if constexpr (is_passing_capture_v<rules_type>) {
                        empty_.reset(sq);
                }
                candidate_action_.release(sq, is_king(sq));
        }

        auto with(Piece const p) noexcept
        {
                candidate_action_.with(p);
        }

        auto into(Piece const p) noexcept
        {
                candidate_action_.into(p);
        }

        auto finalize(std::size_t const sq)
        {
                candidate_action_.dest(sq);
                precedence_duplicates();
        }

        template<class... Args>
        auto pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
                return state_.pieces(std::forward<Args>(args)...);
        }

        auto pieces(none_) const noexcept
        {
                return empty_;
        }

        auto targets() const noexcept
        {
                return initial_targets_ & ~candidate_action_.captured_pieces();
        }

        template<int Direction>
        auto targets() const
        {
                return push_sources<Direction>{}(targets(), pieces(none_c));
        }

        template<class Iterator>
        auto is_target(Iterator it) const
        {
                return targets<board::ray::direction_v<Iterator>.degrees()>().test(*it);
        }

        auto not_occupied(square_type const sq) const
        {
                return empty_.test(sq);
        }

        template<int Direction>
        auto path() const
        {
                auto constexpr jump_start = board::mask::jump_start<board_type>{}(board::Angle{Direction});
                return pieces(none_c) & jump_start;
        }

        template<int Direction>
        auto path(square_type const sq) const
        {
                return path<Direction>().test(sq);
        }

        auto is_last_jumped_king(square_type const sq) const
        {
                return state_.pieces(king_c).test(sq);
        }

        auto with() const noexcept
        {
                return candidate_action_.with();
        }

        auto is_with(Piece const p) const noexcept
        {
                return with() == p;
        }

        auto into() const noexcept
        {
                return candidate_action_.into();
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
                return state_.to_move();
        }

        auto num_captured_pieces() const noexcept
        {
                return 0;
        }

private:
        auto is_king(square_type sq) const
        {
                return state_.pieces(king_c).test(sq);
        }

        auto precedence_duplicates() const
        {
                if constexpr (
                        !is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, keep_duplicates_tag>{}
                ){
                        assert(actions_.empty() || precedence::equal_to{}(candidate_action_, actions_.back()));
                        actions_.push_back(candidate_action_);
                }
                if constexpr (
                        !is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, drop_duplicates_tag>{}
                ){
                        assert(actions_.empty() || precedence::equal_to{}(candidate_action_, actions_.back()));
                        if (actions_.empty() || is_small() || is_unique())
                                actions_.push_back(candidate_action_);
                }
                if constexpr (
                        is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, keep_duplicates_tag>{}
                ){
                        if (actions_.empty() || precedence::equal_to{}(candidate_action_, actions_.back()))
                                return actions_.push_back(candidate_action_);
                        if (precedence::less{}(candidate_action_, actions_.back()))
                                return;
                        assert(precedence::greater{}(candidate_action_, actions_.back()));
                        actions_.clear();
                        actions_.push_back(candidate_action_);
                }
                if constexpr (
                        is_nontrivial_precedence_v<rules_type> &&
                        std::is_same<DuplicatesPolicy, drop_duplicates_tag>{}
                ){
                        if (actions_.empty())
                                return actions_.push_back(candidate_action_);/*
                        if (precedence::equal_to{}(candidate_action_, actions_.back())) {
                                if (is_small() || is_unique())
                                        actions_.push_back(candidate_action_);
                                return;
                        }
                        if (precedence::less{}(candidate_action_, actions_.back()))
                                return;
                        assert(precedence::greater{}(candidate_action_, actions_.back()));
                        actions_.clear();
                        actions_.push_back(candidate_action_);*/

                        switch(precedence::compare{}(candidate_action_, actions_.back())) {
                        case -1 :
                                assert(precedence::less{}(candidate_action_, actions_.back()));
                                return;
                        case  0 :
                                assert(precedence::equal_to{}(candidate_action_, actions_.back()));
                                if (is_small() || is_unique())
                                        actions_.push_back(candidate_action_);
                                return;
                        case +1 :
                                assert(precedence::greater{}(candidate_action_, actions_.back()));
                                actions_.clear();
                                actions_.push_back(candidate_action_);
                                return;
                        default:
                                assert(false);
                        }
                }
        }

        auto is_small() const noexcept
        {
                return candidate_action_.num_captured_pieces() < large_jump_v<rules_type>;
        }

        auto is_unique() const // Throws: Nothing.
        {
                static_assert(std::is_same<DuplicatesPolicy, drop_duplicates_tag>{});
                assert(!actions_.empty());
                assert(precedence::equal_to{}(candidate_action_, actions_.back()));
                return boost::algorithm::none_of(actions_, [&](auto const& a) { return a == candidate_action_; });
        }
};

}       // namespace detail
}       // namespace dctl
