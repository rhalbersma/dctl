#pragma once
#include <dctl/angle.hpp>                       // Angle, is_orthogonal
#include <dctl/board/mask.hpp>                  // JumpStart
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/ray.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/type_traits.hpp>                 // board_type_t, rules_type_t, set_type_t
#include <dctl/utility/stack_vector.hpp>        // DCTL_PP_STACK_RESERVE
#include <dctl/wave/iterator.hpp>
#include <cassert>                              // assert
#include <cstddef>
#include <iterator>                             // begin, end, prev
#include <type_traits>                          // false_type, true_type

namespace dctl {
namespace successor {
namespace detail {

template<Color ToMove, class Position>
class Tracker
{
        using board_type = board_type_t<Position>;
        using rules_type = rules_type_t<Position>;
        using   set_type =   set_type_t<Position>;

        set_type const by_piece_[2];
        set_type initial_targets_;
        set_type remaining_targets_;
        set_type not_occupied_;
        Piece with_{Piece::pawn};
        Piece into_{Piece::pawn};
        set_type king_order_{};
        Arena<std::size_t> sqa_;
        stack_vector<std::size_t> visited_path_ = stack_vector<std::size_t>(Alloc<std::size_t>{sqa_});
        Arena<std::size_t> pca_;
        stack_vector<std::size_t> removed_pieces_ = stack_vector<std::size_t>(Alloc<std::size_t>{pca_});

        auto invariant() const
        {
                return remaining_targets_.is_subset_of(initial_targets_);
        }

public:
        explicit Tracker(Position const& p)
        :
                by_piece_{p.pieces(!ToMove, Piece::pawn), p.pieces(!ToMove, Piece::king)},
                initial_targets_(p.pieces(!ToMove)),
                remaining_targets_(initial_targets_),
                not_occupied_(p.not_occupied())
        {
                visited_path_.reserve(DCTL_PP_STACK_RESERVE);
                removed_pieces_.reserve(DCTL_PP_STACK_RESERVE);
                assert(invariant());
        }

        auto launch(std::size_t sq)
        {
                visited_path_.push_back(sq);
                not_occupied_.set(sq);
        }

        auto finish()
        {
                not_occupied_.reset(from());
                visited_path_.pop_back();
        }

        auto capture(std::size_t sq)
        {
                capture_dispatch(sq, is_en_passant_jump_removal_t<rules_type>{});
        }

        auto release()
        {
                release_dispatch(last_piece(), is_en_passant_jump_removal_t<rules_type>{});
        }

        auto last_visit(std::size_t sq)
        {
                visited_path_.back() = sq;
        }

        auto visit(std::size_t sq)
        {
                visited_path_.push_back(sq);
        }

        auto leave()
        {
                visited_path_.pop_back();
        }

        auto toggle_king_targets() noexcept
        {
                static_assert(!is_pawn_jump_king_v<rules_type>, "");
                initial_targets_ = remaining_targets_ ^= by_piece_[static_cast<std::size_t>(Piece::king)];
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

        auto path(std::size_t sq) const
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
        auto path(int sq) const
        {
                return path<Direction>().test(sq);
        }

        auto is_king(std::size_t sq) const
        {
                return by_piece_[static_cast<std::size_t>(Piece::king)].test(sq);
        }

        auto king_order() const
        {
                return king_order_;
        }

        auto captured() const noexcept
        {
                return initial_targets_ ^ remaining_targets_;
        }

        auto captured(Piece p) const noexcept
        {
                return captured() & by_piece_[static_cast<std::size_t>(p)];
        }

        auto from() const
        {
                assert(!visited_path_.empty());
                return visited_path_.front();
        }

        auto dest() const
        {
                assert(2 <= visited_path_.size());
                return visited_path_.back();
        }

        auto to_move() const noexcept
        {
                return ToMove;
        }

        auto with() const
        {
                return with_;
        }

        auto is_with(Piece p) const noexcept
        {
                return with() == p;
        }

        auto into() const
        {
                return into_;
        }

        auto is_into(Piece p) const noexcept
        {
                return into() == p;
        }

private:
        // apres-fini jump removal
        auto capture_dispatch(std::size_t sq, std::false_type)
        {
                capture_impl(sq);
        }

        // en-passant jump removal
        auto capture_dispatch(std::size_t sq, std::true_type)
        {
                not_occupied_.set(sq);
                capture_impl(sq);
        }

        void capture_impl(std::size_t sq)
        {
                if (is_king(sq))
                        king_order_.set(set_type::size() - 1 - num_captured());
                removed_pieces_.push_back(sq);
                remaining_targets_.reset(sq);
        }

        // apres-fini jump removal
        void release_dispatch(std::size_t sq, std::false_type)
        {
                release_impl(sq);
        }

        // en-passant jump removal
        void release_dispatch(std::size_t sq, std::true_type)
        {
                release_impl(sq);
                not_occupied_.reset(sq);
        }

        void release_impl(std::size_t sq)
        {
                remaining_targets_.set(sq);
                removed_pieces_.pop_back();
                if (is_king(sq))
                        king_order_.reset(set_type::size() - 1 - num_captured());
        }

        template<int Direction>
        static wave::Iterator<board_type, Direction> along_wave(set_type const& s)
        {
                return wave::make_iterator<board_type, Direction>(s);
        }

        auto num_captured() const
        {
                return removed_pieces_.size();
        }

        auto last_piece() const
        {
                assert(!removed_pieces_.empty());
                return removed_pieces_.back();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
