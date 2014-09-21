#pragma once
#include <dctl/angle.hpp>               // Angle, is_orthogonal
#include <dctl/bit.hpp>
#include <dctl/board/board.hpp>
#include <dctl/position/promotion.hpp>
#include <dctl/position/unary_projections.hpp>
#include <dctl/pieces/pieces.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/successor/select/jump.hpp>
#include <dctl/utility/total_order.hpp>
#include <dctl/ray.hpp>
#include <dctl/wave/iterator.hpp>
#include <dctl/board/mask.hpp>
#include <dctl/type_traits.hpp>         // board_type_t, rules_type_t
#include <cassert>                      // assert
#include <iterator>                     // begin, end, prev
#include <type_traits>                  // integral_constant, is_same, false_type, true_type
#include <dctl/utility/stack_vector.hpp>                // DCTL_PP_STACK_RESERVE
#include <vector>
#include <iostream>

namespace dctl {
namespace successor {
namespace with {

struct king {};
struct pawn {};

}       // namespace with

template<bool Color, class Position>
class Tracker
{
public:
        // constructors

        explicit Tracker(Position const& p)
        :
                king_targets_(passive_kings(p)),
                initial_targets_(passive_pieces(p)),
                remaining_targets_(initial_targets_),
                not_occupied_(p.not_occupied())
        {
                visited_path_.reserve(DCTL_PP_STACK_RESERVE);
                removed_pieces_.reserve(DCTL_PP_STACK_RESERVE);
                assert(invariant());
        }

        using Rules = rules_type_t<Position>;
        using rules_type = Rules;
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;

        // modifiers

        auto launch(int sq)
        {
                visited_path_.push_back(sq);
                not_occupied_.set(sq);
        }

        auto finish()
        {
                not_occupied_.reset(from_sq());
                visited_path_.pop_back();
        }

        auto capture(int sq)
        {
                // tag dispatching on jump removal
                capture_dispatch(sq, is_en_passant_jump_removal_t<Rules>{});
        }

        auto release()
        {
                // tag dispatching on jump removal
                release_dispatch(last_piece(), is_en_passant_jump_removal_t<Rules>{});
        }

        auto last_visit(int sq)
        {
                visited_path_.back() = sq;
        }

        auto visit(int sq)
        {
                visited_path_.push_back(sq);
        }

        auto leave()
        {
                visited_path_.pop_back();
        }

        auto toggle_king_targets() noexcept
        {
                static_assert(!is_pawn_jump_king_v<Rules>, "");
                initial_targets_ = remaining_targets_ ^= king_targets_;
        }

        auto toggle_is_with_king() noexcept
        {
                is_with_king_ ^= true;
        }

        auto toggle_is_promotion() noexcept
        {
                is_promotion_ ^= true;
        }

        // observers

        template<class Iterator>
        auto targets(Iterator it) const
        {
                return targets<ray::direction_v<Iterator>>().test(*it);
        }

        template<int Direction>
        auto targets() const
        {
                return remaining_targets_ & Set(*std::prev(along_wave<Direction>(path())));
        }

        auto path() const
        {
                return not_occupied_;
        }

        auto path(int sq) const
        {
                return not_occupied_.test(sq);
        }

        template<int Direction>
        auto path() const
        {
                auto constexpr jump_start = board::JumpStart<Board>::mask(Angle{Direction});
                return path() & jump_start;
        }

        template<int Direction>
        auto path(int sq) const
        {
                return path<Direction>().test(sq);
        }

        auto is_king(int sq) const
        {
                return king_targets_.test(sq);
        }

        auto king_order() const
        {
                return king_order_;
        }

        auto captured_pieces() const noexcept
        {
                return initial_targets_ ^ remaining_targets_;
        }

        auto captured_kings() const noexcept
        {
                return captured_pieces() & king_targets_;
        }

        auto from_sq() const
        {
                assert(!visited_path_.empty());
                return visited_path_.front();
        }

        auto dest_sq() const
        {
                assert(2 <= visited_path_.size());
                return visited_path_.back();
        }

        auto is_with_king() const noexcept
        {
                return is_with_king_;
        }

        auto is_promotion() const noexcept
        {
                return is_promotion_;
        }

        auto active_color() const noexcept
        {
                return Color;
        }

private:
        // modifiers

        // apres-fini jump removal
        auto capture_dispatch(int sq, std::false_type)
        {
                capture_impl(sq);
        }

        // en-passant jump removal
        auto capture_dispatch(int sq, std::true_type)
        {
                not_occupied_.set(sq);
                capture_impl(sq);
        }

        void capture_impl(int sq)
        {
                if (is_king(sq))
                        king_order_.set(Set::size() - 1 - num_pieces());
                removed_pieces_.push_back(sq);
                remaining_targets_.reset(sq);
        }

        // apres-fini jump removal
        void release_dispatch(int sq, std::false_type)
        {
                release_impl(sq);
        }

        // en-passant jump removal
        void release_dispatch(int sq, std::true_type)
        {
                release_impl(sq);
                not_occupied_.reset(sq);
        }

        void release_impl(int sq)
        {
                remaining_targets_.set(sq);
                removed_pieces_.pop_back();
                if (is_king(sq))
                        king_order_.reset(Set::size() - 1 - num_pieces());
        }

        template<int Direction>
        static wave::Iterator<Board, Direction> along_wave(Set const& s)
        {
                return wave::make_iterator<Board, Direction>(s);
        }

        auto num_pieces() const
        {
                return static_cast<int>(removed_pieces_.size());
        }

        auto last_piece() const
        {
                assert(!removed_pieces_.empty());
                return removed_pieces_.back();
        }

        // contracts

        auto invariant() const
        {
                return remaining_targets_.is_subset_of(initial_targets_);
        }

        // representation

        Set const king_targets_;
        Set initial_targets_;
        Set remaining_targets_;
        Set not_occupied_;
        Set king_order_{};
        Arena<int> sqa_;
        Arena<int> pca_;
        stack_vector<int> visited_path_ = stack_vector<int>(Alloc<int>{sqa_});
        stack_vector<int> removed_pieces_ = stack_vector<int>(Alloc<int>{pca_});
        bool is_with_king_{};
        bool is_promotion_{};
};

}       // namespace successor
}       // namespace dctl
