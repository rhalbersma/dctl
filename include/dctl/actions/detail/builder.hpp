#pragma once
#include <dctl/board/angle.hpp>                 // angle, is_orthogonal
#include <dctl/board/mask.hpp>                  // JumpStart
#include <dctl/board/ray.hpp>
#include <dctl/board/wave/iterator.hpp>
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/state/piece_placement/pieces.hpp>
#include <dctl/utility/type_traits.hpp>         // board_t, rules_t, set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type, value_t
#include <algorithm>                            // find_if
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <iterator>                             // begin, end, prev
#include <experimental/type_traits>

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class DuplicatesPolicy, class State, class SequenceContainer>
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
                initial_targets_ ^= pieces<!ToMove, Piece::king>(state);
        }

        auto make_launch(std::size_t const sq)
        {
                candidate_action.set_from(sq);
                not_occupied_.set(sq);
        }

        auto undo_launch(std::size_t const sq)
        {
                not_occupied_.reset(sq);
        }

        auto capture_piece(std::size_t const sq)
        {
                capture_piece_dispatch(sq, capture_category_t<rules_type>{});
        }

        auto release_piece(std::size_t const sq)
        {
                release_piece_dispatch(sq, capture_category_t<rules_type>{});
        }

        auto set_with(Piece const p) noexcept
        {
                candidate_action.set_with(p);
        }

        auto set_into(Piece const p) noexcept
        {
                candidate_action.set_into(p);
        }

        auto finalize(std::size_t const dest_sq)
        {
                candidate_action.set_dest(dest_sq);
                precedence_duplicates_dispatch(precedence_category_t<rules_type>{}, DuplicatesPolicy{});
        }

        auto active_pawns() const noexcept
        {
                return pieces<ToMove, Piece::pawn>(state);
        }

        auto active_kings() const noexcept
        {
                return pieces<ToMove, Piece::king>(state);
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
                return current_targets() & set_type(*std::prev(along_wave<Direction>(not_occupied())));
        }

        auto not_occupied() const
        {
                return not_occupied_;
        }

        auto not_occupied(square_type sq) const
        {
                return not_occupied_.test(sq);
        }

        template<int Direction>
        auto path() const
        {
                auto constexpr jump_start = board::JumpStart<board_type>::mask(angle{Direction});
                return not_occupied() & jump_start;
        }

        template<int Direction>
        auto path(square_type sq) const
        {
                return path<Direction>().test(sq);
        }

        auto is_last_jumped_king(square_type sq) const
        {
                return pieces<Piece::king>(state).test(sq);
        }

        auto with() const noexcept
        {
                return candidate_action.with();
        }

        auto is_with(Piece p) const noexcept
        {
                return with() == p;
        }

        auto is_with_king() const noexcept
        {
                return is_with(Piece::king);
        }

        auto into() const noexcept
        {
                return candidate_action.into();
        }

        auto is_into(Piece p) const noexcept
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

        auto is_to_move(Color c) const noexcept
        {
                return to_move() == c;
        }

        auto num_captured_pieces() const noexcept
        {
                return 0;
        }

private:
        auto capture_piece_dispatch(std::size_t const sq, stopped_capture_tag)
        {
                candidate_action.capture_piece(sq, is_king(sq));
        }

        auto capture_piece_dispatch(std::size_t const sq, passing_capture_tag)
        {
                candidate_action.capture_piece(sq, is_king(sq));
                not_occupied_.set(sq);
        }

        auto release_piece_dispatch(std::size_t const sq, stopped_capture_tag)
        {
                candidate_action.release_piece(sq, is_king(sq));
        }

        auto release_piece_dispatch(std::size_t const sq, passing_capture_tag)
        {
                candidate_action.release_piece(sq, is_king(sq));
                not_occupied_.reset(sq);
        }

        auto is_king(square_type sq) const
        {
                return pieces<Piece::king>(state).test(sq);
        }

        template<int Direction>
        static auto along_wave(set_type const& s)
        {
                return board::wave::make_iterator<board_type, Direction>(s);
        }

        auto precedence_duplicates_dispatch(trivial_precedence_tag, keep_duplicates_tag) const
        {
                assert(actions.empty() || precedence::equal_to{}(candidate_action, actions.back()));
                actions.push_back(candidate_action);
        }

        auto precedence_duplicates_dispatch(trivial_precedence_tag, drop_duplicates_tag) const
        {
                if (actions.empty())
                        return actions.push_back(candidate_action);
                add_if_not_duplicate();
        }

        auto precedence_duplicates_dispatch(nontrivial_precedence_tag, keep_duplicates_tag) const
        {
                if (actions.empty() || precedence::equal_to{}(candidate_action, actions.back()))
                        return actions.push_back(candidate_action);

                if (precedence::greater{}(candidate_action, actions.back())) {
                        actions.clear();
                        return actions.push_back(candidate_action);
                }

                assert(precedence::less{}(candidate_action, actions.back()));
        }

        auto precedence_duplicates_dispatch(nontrivial_precedence_tag, drop_duplicates_tag) const
        {
                if (actions.empty())
                        return actions.push_back(candidate_action);

                if (precedence::equal_to{}(candidate_action, actions.back()))
                        return add_if_not_duplicate();

                if (precedence::greater{}(candidate_action, actions.back())) {
                        actions.clear();
                        return actions.push_back(candidate_action);
                }

                assert(precedence::less{}(candidate_action, actions.back()));
        }

        auto add_if_not_duplicate() const
        {
                static_assert(std::experimental::is_same_v<DuplicatesPolicy, drop_duplicates_tag>);
                assert(!actions.empty());
                assert(precedence::equal_to{}(candidate_action, actions.back()));
                if (!is_large(candidate_action) || std::find(actions.cbegin(), actions.cend(), candidate_action) == actions.cend())
                        actions.push_back(candidate_action);
        }

        auto is_large(action_type const& a) const noexcept
        {
                return large_jump_v<rules_type> <= a.num_captured_pieces();
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
