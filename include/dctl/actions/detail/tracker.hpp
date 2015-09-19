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
#include <cstddef>
#include <iterator>                             // begin, end, prev

namespace dctl {
namespace actions {
namespace detail {

template<Color ToMove, class Unique, class State>
class Tracker
{
public:
        using  state_type = State;
        using  board_type = board_t<state_type>;
        using  rules_type = rules_t<state_type>;
        using    set_type =   set_t<state_type>;
        using square_type =  std::size_t;

private:
        set_type const by_piece_[2];
        set_type initial_targets_;
        set_type remaining_targets_;
        set_type not_occupied_;
        Piece with_{Piece::pawn};
        Piece into_{Piece::pawn};
        set_type piece_order_;
        bool is_large_;
        //static std::vector<square_type> visited_squares_;
        //static std::vector<square_type> jumped_squares_;
        util::bounded_vector<square_type, 64> visited_squares_;
        util::bounded_vector<square_type, 64> jumped_squares_;

        auto invariant() const
        {
                return remaining_targets_.is_subset_of(initial_targets_);
        }

public:
        explicit Tracker(state_type const& s)
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

        auto release()
        {
                release_dispatch(last_jumped_square(), capture_category_t<rules_type>{});
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

        auto is_last_jumped(Piece p) const
        {
                return by_piece(p).test(last_jumped_square());
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
                return jumped_squares_.size();
        }

        auto num_captured(Piece p) const noexcept
        {
                return captured(p).size();
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

        void capture_impl(square_type sq)
        {
                if (is_king(sq))
                        piece_order_.set(from_back(num_captured()));
                jumped_squares_.push_back(sq);
                remaining_targets_.reset(sq);
        }

        void release_dispatch(square_type sq, stopped_capture_tag)
        {
                release_impl(sq);
        }

        void release_dispatch(square_type sq, passing_capture_tag)
        {
                release_impl(sq);
                not_occupied_.reset(sq);
        }

        void release_impl(square_type sq)
        {
                remaining_targets_.set(sq);
                jumped_squares_.pop_back();
                if (is_king(sq))
                        piece_order_.reset(from_back(num_captured()));
        }

        template<int Direction>
        static wave::Iterator<board_type, Direction> along_wave(set_type const& s)
        {
                return wave::make_iterator<board_type, Direction>(s);
        }

        auto is_king(square_type sq) const
        {
                return by_piece(Piece::king).test(sq);
        }

        auto last_jumped_square() const
        {
                assert(!jumped_squares_.empty());
                return jumped_squares_.back();
        }

        auto by_piece(Piece p) const
        {
                return by_piece_[xstd::to_underlying_type(p)];
        }

        auto from_back(std::size_t n) const
        {
                return set_type::size() - 1 - n;
        }

        auto is_large() const noexcept
        {
                return large_jump_v<rules_type> <= num_captured();
        }

        template<class SequenceContainer>
        auto precedence_dispatch(SequenceContainer& moves, std::false_type) const
        {
                if (moves.empty())
                        return moves.emplace_back(*this);

                if (precedence::less_t<rules_type>{}(moves.front(), *this)) {
                        moves.clear();
                        return moves.emplace_back(*this);
                }

                if (precedence::not_equal_to_t<rules_type>{}(moves.front(), *this))
                        return;

                moves.emplace_back(*this);
                assert(2 <= moves.size());
                uniqueness_dispatch(moves, std::false_type{}, Unique{});
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
                if (is_large() && std::find(begin(moves), end(moves), moves.back()) != std::prev(end(moves)))
                        moves.pop_back();
        }
};
/*
template<Color ToMove, class State>
std::vector<std::size_t> Tracker<ToMove, State>::visited_squares_;

template<Color ToMove, class State>
std::vector<std::size_t> Tracker<ToMove, State>::jumped_squares_;
*/
}       // namespace detail
}       // namespace actions
}       // namespace dctl
