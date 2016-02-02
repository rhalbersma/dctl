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

template<Color ToMove, class DuplicatesPolicy, class State>
class Counter
{
public:
        using  board_type = board_t<State>;
        using  rules_type = rules_t<State>;
        using    set_type =   set_t<State>;
        using square_type = std::size_t;

private:
        set_type const by_piece_[2];
        set_type initial_targets_;
        set_type remaining_targets_;
        set_type not_occupied_;
        square_type from_;
        Piece with_{Piece::pawn};
        Piece into_{Piece::pawn};

        auto invariant() const
        {
                return remaining_targets_.is_subset_of(initial_targets_);
        }

public:
        explicit Counter(State const& s)
        :
                by_piece_{s.pieces(!ToMove, Piece::pawn), s.pieces(!ToMove, Piece::king)},
                initial_targets_(s.pieces(!ToMove)),
                remaining_targets_(initial_targets_),
                not_occupied_(s.not_occupied())
        {
                assert(invariant());
        }

        auto launch(square_type sq)
        {
                from_ = sq;
                not_occupied_.set(sq);
        }

        auto finish(square_type)
        {
        }

        auto clear()
        {
                not_occupied_.reset(from());
        }

        auto capture(square_type sq)
        {
                capture_dispatch(sq, capture_category_t<rules_type>{});
        }

        auto release(square_type sq)
        {
                release_dispatch(sq, capture_category_t<rules_type>{});
        }

        auto visit(square_type /* sq */)
        {
                //visited_squares_.push_back(sq);
        }

        auto leave()
        {
                //visited_squares_.pop_back();
        }

        auto toggle_king_targets() noexcept
        {
                static_assert(is_superior_rank_jump_v<rules_type>);
                initial_targets_ ^= by_piece(Piece::king);
                remaining_targets_ ^= by_piece(Piece::king);
        }

        auto set_with(Piece p) noexcept
        {
                with_ = p;
        }

        auto set_into(Piece p) noexcept
        {
                into_ = p;
        }

        auto remaining_targets() const
        {
                return remaining_targets_;
        }

        template<class Iterator>
        auto targets(Iterator it) const
        {
                return targets<ray::direction_v<Iterator>>().test(*it);
        }

        template<int Direction>
        auto targets() const
        {
                return remaining_targets_ & set_type(*std::prev(along_wave<Direction>(path())));
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

        auto is_last_jumped(Piece p, square_type sq) const
        {
                return by_piece(p).test(sq);
        }

        auto captured() const noexcept
        {
                return initial_targets_ ^ remaining_targets_;
        }

        auto captured(Piece p) const noexcept
        {
                return captured() & by_piece(p);
        }

        auto captured_kings() const noexcept
        {
                return captured(Piece::king);
        }

        auto from() const
        {
                return from_;
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
                return ToMove;
        }

        auto is_to_move(Color c) const noexcept
        {
                return to_move() == c;
        }

        template<class SequenceContainer>
        auto append_to(SequenceContainer& actions) const
        {
                ++actions;
        }

private:
        auto capture_dispatch(square_type sq, stopped_capture_tag)
        {
                capture_impl(sq);
        }

        auto capture_dispatch(square_type sq, passing_capture_tag)
        {
                not_occupied_.set(sq);
                capture_impl(sq);
        }

        auto capture_impl(square_type sq)
        {
                remaining_targets_.reset(sq);
        }

        auto release_dispatch(square_type sq, stopped_capture_tag)
        {
                release_impl(sq);
        }

        auto release_dispatch(square_type sq, passing_capture_tag)
        {
                release_impl(sq);
                not_occupied_.reset(sq);
        }

        auto release_impl(square_type sq)
        {
                remaining_targets_.set(sq);
        }

        template<int Direction>
        static auto along_wave(set_type const& s)
        {
                return wave::make_iterator<board_type, Direction>(s);
        }

        auto is_king(square_type sq) const
        {
                return by_piece(Piece::king).test(sq);
        }

        auto by_piece(Piece p) const
        {
                return by_piece_[xstd::to_underlying_type(p)];
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
