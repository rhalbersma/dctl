#pragma once
#include <dctl/board/angle.hpp>                 // Angle, is_orthogonal
#include <dctl/board/mask.hpp>                  // JumpStart
#include <dctl/board/ray.hpp>
#include <dctl/board/wave/iterator.hpp>
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/utility/type_traits.hpp>         // board_t, rules_t, set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <boost/container/static_vector.hpp>
#include <algorithm>                            // find_if
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <iterator>                             // begin, end, prev
#include <type_traits>                          // bool_constant, false_type, true_type

namespace dctl {
namespace core {
namespace detail {

template<class DuplicatesPolicy, class State, class Sequence>
class Builder
{
public:
        using  board_type = board_t<State>;
        using  rules_type = rules_t<State>;
        using    set_type =   set_t<State>;
        using square_type = std::size_t;
        using action_type = typename Sequence::value_type;

private:
        State const& state;
        Sequence& actions;
        set_type initial_targets_;
        set_type not_occupied_;
        Piece with_{Piece::pawn};
        Piece into_{Piece::pawn};

public:
        explicit Builder(State const& s, Sequence& a)
        :
                state{s},
                actions{a},
                initial_targets_(state.pieces(!state.to_move())),
                not_occupied_(state.not_occupied())
        {}

        auto make_launch(square_type sq)
        {
                not_occupied_.set(sq);
        }

        auto undo_launch(square_type sq)
        {
                not_occupied_.reset(sq);
        }

        auto toggle_king_targets() noexcept
        {
                static_assert(is_superior_rank_jump_v<rules_type>);
                initial_targets_ ^= state.pieces(!state.to_move(), Piece::king);
        }

        auto set_with(Piece p) noexcept
        {
                with_ = p;
        }

        auto set_into(Piece p) noexcept
        {
                into_ = p;
        }

        auto targets() const
        {
                return initial_targets_;
        }

        template<class Iterator>
        auto targets(Iterator it, action_type current) const
        {
                return targets<ray::direction_v<Iterator>>(current).test(*it);
        }

        template<int Direction>
        auto targets(action_type current) const
        {
                return initial_targets_ & ~current.captured() & set_type(*std::prev(along_wave<Direction>(path())));
        }

        auto path() const
        {
                return not_occupied_;
        }

        auto path(square_type sq) const
        {
                return not_occupied_.test(sq);
        }

        template<int Direction>
        auto path() const
        {
                auto constexpr jump_start = board::JumpStart<board_type>::mask(Angle{Direction});
                return path() & jump_start;
        }

        template<int Direction>
        auto path(square_type sq) const
        {
                return path<Direction>().test(sq);
        }

        auto is_last_jumped_king(square_type sq) const
        {
                return state.pieces(Piece::king).test(sq);
        }

        auto with() const noexcept
        {
                return with_;
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
                return into_;
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

        auto num_captured() const noexcept
        {
                return 0;
        }

        auto append_to() const
        {
                precedence_dispatch(precedence_category_t<rules_type>{});
        }

        auto append_to(action_type current) const
        {
                precedence_dispatch(current, precedence_category_t<rules_type>{});
        }

private:
        template<int Direction>
        static auto along_wave(set_type const& s)
        {
                return wave::make_iterator<board_type, Direction>(s);
        }

        auto is_king(square_type sq) const
        {
                return state.pieces(Piece::king).test(sq);
        }

        auto reverse_index(std::size_t n) const noexcept
        {
                return set_type::size() - 1 - n;
        }

        auto is_large(std::size_t n) const noexcept
        {
                return large_jump_v<rules_type> <= n;
        }

/* need: begin/end, clear/empty, push_back/pop_back/back */
/* modeled by std::vector, std::deque and std::list              */

        auto precedence_dispatch(action_type const& current, trivial_precedence_tag) const
        {
                duplicates_dispatch(current, trivial_precedence_tag{}, DuplicatesPolicy{});
        }

        auto precedence_dispatch(action_type const& current, nontrivial_precedence_tag) const
        {
                if (actions.empty())
                        return actions.push_back(current);

                if (equal_to(current, actions.back()))
                        return duplicates_dispatch(current, nontrivial_precedence_tag{}, DuplicatesPolicy{});

                if (less(current, actions.back()))
                        return;

                actions.clear();
                actions.push_back(current);
        }

        template<class PrecedenceCategory>
        auto duplicates_dispatch(action_type const& current, PrecedenceCategory, keep_duplicates_tag) const
        {
                actions.push_back(current);
        }

        auto duplicates_dispatch(action_type const& current, trivial_precedence_tag, drop_duplicates_tag) const
        {
                if (actions.empty())
                        return actions.push_back(current);
                drop_duplicates(current);
        }

        auto duplicates_dispatch(action_type const& current, nontrivial_precedence_tag, drop_duplicates_tag) const
        {
                drop_duplicates(current);
        }

        auto drop_duplicates(action_type const& current) const
        {
                static_assert(DuplicatesPolicy{});
                assert(!actions.empty());
                if (!is_large(current.num_captured()) || std::find(actions.begin(), actions.end(), current) == actions.end())
                        actions.push_back(current);
        }

        auto equal_to(action_type const& a1, action_type const& a2) const
        {
                return precedence::equal_to<rules_type>{}(a1, a2);
        }

        auto less(action_type const& a1, action_type const& a2) const
        {
                return precedence::less<rules_type>{}(a1, a2);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
