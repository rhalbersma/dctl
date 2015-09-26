#pragma once
#include <dctl/board/angle.hpp>                 // Angle, is_orthogonal
#include <dctl/board/mask.hpp>                  // JumpStart
#include <dctl/board/ray.hpp>
#include <dctl/board/wave/iterator.hpp>
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/utility/bounded_vector.hpp>
#include <dctl/utility/type_traits.hpp>         // board_t, rules_t, set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <algorithm>                            // find_if
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <iterator>                             // begin, end, prev
#include <type_traits>                          // bool_constant (C++1z), false_type, true_type

namespace dctl {
namespace actions {
namespace detail {

template<Color ToMove, class Unique, class State>
class Tracker
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
        Piece with_{Piece::pawn};
        Piece into_{Piece::pawn};
        set_type piece_order_;
        util::bounded_vector<square_type> visited_squares_;
        //util::bounded_vector<square_type> jumped_squares_;

        auto invariant() const
        {
                return remaining_targets_.is_subset_of(initial_targets_);
        }

public:
        explicit Tracker(State const& s)
        :
                by_piece_{s.pieces(!ToMove, Piece::pawn), s.pieces(!ToMove, Piece::king)},
                initial_targets_(s.pieces(!ToMove)),
                remaining_targets_(initial_targets_),
                not_occupied_(s.not_occupied()),
                piece_order_{}
        {
                assert(invariant());
        }

        auto launch(square_type sq)
        {
                assert(visited_squares_.empty());
                visited_squares_.push_back(sq);
                not_occupied_.set(sq);
        }

        auto finish()
        {
                not_occupied_.reset(from());
                visited_squares_.pop_back();
                assert(visited_squares_.empty());
        }

        auto capture(square_type sq)
        {
                capture_dispatch(sq, capture_category_t<rules_type>{});
        }

        auto release(square_type sq)
        {
                release_dispatch(sq, capture_category_t<rules_type>{});
        }

        auto visit(square_type sq)
        {
                visited_squares_.push_back(sq);
        }

        auto leave()
        {
                visited_squares_.pop_back();
        }

        auto toggle_king_targets() noexcept
        {
                static_assert(!is_pawn_jump_king_v<rules_type>, "");
                initial_targets_ = remaining_targets_ ^= by_piece(Piece::king);
        }

        auto set_with(Piece p) noexcept
        {
                with_ = p;
        }

        auto set_into(Piece p) noexcept
        {
                into_ = p;
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

        auto from() const
        {
                assert(!visited_squares_.empty());
                return visited_squares_.front();
        }

        auto dest() const
        {
                assert(2 <= visited_squares_.size());
                return visited_squares_.back();
        }

        auto set_dest(square_type sq)
        {
                assert(2 <= visited_squares_.size());
                visited_squares_.back() = sq;
        }

        auto to_move() const noexcept
        {
                return ToMove;
        }

        auto is_to_move(Color c) const noexcept
        {
                return to_move() == c;
        }

        auto with() const noexcept
        {
                return with_;
        }

        auto is_with(Piece p) const noexcept
        {
                return with() == p;
        }

        auto into() const noexcept
        {
                return into_;
        }

        auto is_into(Piece p) const noexcept
        {
                return into() == p;
        }

        auto num_captured() const noexcept
        {
                return captured().count();
        }

        auto num_captured(Piece p) const noexcept
        {
                return captured(p).count();
        }

        auto piece_order() const
        {
                return piece_order_;
        }

        template<class SequenceContainer>
        auto append_to(SequenceContainer& moves) const
        {
                precedence_dispatch(moves, precedence::is_trivial_t<rules_type>{});
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
                if (is_king(sq))
                        piece_order_.set(reverse_index(num_captured()));
                //jumped_squares_.push_back(sq);
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
                //jumped_squares_.pop_back();
                if (is_king(sq))
                        piece_order_.reset(reverse_index(num_captured()));
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

        auto reverse_index(std::size_t n) const noexcept
        {
                return set_type::size() - 1 - n;
        }

        auto is_large(std::size_t n) const noexcept
        {
                return large_jump_v<rules_type> <= n;
        }

        template<class SequenceContainer>
        auto precedence_dispatch(SequenceContainer& moves, std::false_type) const
        {
                if (moves.empty())
                        return moves.emplace_back(*this);

                if (precedence::less_t<rules_type>{}(*this, moves.front()))
                        return;

                if (precedence::equal_to_t<rules_type>{}(*this, moves.front())) {
                        moves.emplace_back(*this);
                        return uniqueness_dispatch(moves, std::false_type{}, Unique{});
                }

                moves.clear();
                moves.emplace_back(*this);
        }

        template<class SequenceContainer>
        auto precedence_dispatch(SequenceContainer& moves, std::true_type) const
        {
                moves.emplace_back(*this);
                uniqueness_dispatch(moves, std::true_type{}, Unique{});
        }

        template<class SequenceContainer, bool B>
        auto uniqueness_dispatch(SequenceContainer&, std::bool_constant<B>, std::false_type) const
        {
                // no-op
        }

        template<class SequenceContainer>
        auto uniqueness_dispatch(SequenceContainer& moves, std::false_type, std::true_type) const
        {
                unique(moves);
        }

        template<class SequenceContainer>
        auto uniqueness_dispatch(SequenceContainer& moves, std::true_type, std::true_type) const
        {
                if (2 <= moves.size())
                        unique(moves);
        }

        template<class SequenceContainer>
        auto unique(SequenceContainer& moves) const
        {
                static_assert(Unique::value, "");
                assert(2 <= moves.size());
                if (is_large(moves.front().num_captured()) && std::find(begin(moves), end(moves), moves.back()) != std::prev(end(moves)))
                        moves.pop_back();
        }
};

}       // namespace detail
}       // namespace actions
}       // namespace dctl
