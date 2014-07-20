#pragma once
#include <dctl/angle.hpp>               // Angle, is_orthogonal
#include <dctl/bit.hpp>
#include <dctl/board/board.hpp>
#include <dctl/position/promotion.hpp>
#include <dctl/position/unary_projections.hpp>
#include <dctl/pieces/pieces.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/rules/types.hpp>
#include <dctl/successor/propagate_fwd.hpp>
#include <dctl/successor/select/jump.hpp>
#include <dctl/successor/value.hpp>
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

namespace dctl {
namespace successor {
namespace with {

struct king {};
struct pawn {};

}       // namespace with

template<class Position>
class Propagate<select::jump, Position>
{
public:
        // structors

        explicit Propagate(Position const& p)
        :
                king_targets_(passive_kings(p)),
                initial_targets_(passive_pieces(p)),
                remaining_targets_(initial_targets_),
                not_occupied_(p.not_occupied())
        {
                visited_path_.reserve(DCTL_PP_STACK_RESERVE);
                removed_pieces_.reserve(DCTL_PP_STACK_RESERVE);
                ordered_kings_.reserve(DCTL_PP_STACK_RESERVE);
                assert(invariant());
        }

        using Rules = rules_type_t<Position>;
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;

        // modifiers

        void launch(int sq)
        {
                visited_path_.push_back(sq);
                not_occupied_.set(sq);
        }

        void finish()
        {
                not_occupied_.reset(from_sq());
                visited_path_.pop_back();
        }

        void capture(int sq)
        {
                // tag dispatching on jump removal
                capture_dispatch(sq, is_en_passant_jump_removal_t<Rules>{});
        }

        void release()
        {
                // tag dispatching on jump removal
                release_dispatch(last_piece(), is_en_passant_jump_removal_t<Rules>{});
        }

        void toggle_king_targets()
        {
                static_assert(!is_pawn_jump_king_v<Rules>, "");
                initial_targets_ = remaining_targets_ ^= king_targets_;
        }

        void toggle_with_king()
        {
                is_with_king_ ^= true;
        }

        void toggle_promotion()
        {
                is_promotion_ ^= true;
        }

        void improve()
        {
                static_assert(is_jump_precedence_v<Rules>, "");
                assert(best_ < current_);
                best_ = current_;
        }

        template<bool Color, class Sequence>
        void add_king_jump(int dest, Sequence& moves) const
        {
                visited_path_.push_back(dest);
/*
                std::cout << "TRACING JUMP: ";
                std::cout << "squares={";
                for (auto&& s : squares_) std::cout << Board::numeric_from_bit(s) << ",";
                std::cout << "}, ";
                std::cout << "pieces={";
                for (auto&& p : pieces_) std::cout << Board::numeric_from_bit(p) << ", ";
                std::cout << "}\n";
*/
                moves.emplace_back(
                        captured_pieces(),
                        captured_kings(with::king{}),
                        from_sq(),
                        dest_sq(),
                        Color
                );

                visited_path_.pop_back();
        }

        template<bool Color, class WithPiece, class Sequence>
        void add_pawn_jump(int dest, Sequence& moves) const
        {
                visited_path_.push_back(dest);

                /*
                std::cout << "TRACING JUMP: ";
                std::cout << "squares={";
                for (auto&& s : squares_) std::cout << Board::numeric_from_bit(s) << ",";
                std::cout << "}, ";
                std::cout << "pieces={";
                for (auto&& p : pieces_) std::cout << Board::numeric_from_bit(p) << ", ";
                std::cout << "}\n";
*/
                moves.emplace_back(
                        captured_pieces(),
                        captured_kings(WithPiece{}),
                        from_sq(),
                        dest_sq(),
                        is_promotion<Color>(dest_sq(), WithPiece{}),
                        Color
                );

                visited_path_.pop_back();
        }

        template<class Sequence>
        auto handle_precedence(Sequence& moves)
        {
                current_ = Value<Rules>{*this};

                if (moves.empty())      goto START1;
                if (best_ <  current_)  goto START0;
                if (best_ == current_)  goto START2;

                return false;

                START0 : moves.clear();
                START1 : best_ = current_;
                START2 : return true;
        }

        // queries

        template<int Direction>
        auto targets_with_king() const
        {
                return remaining_targets<Direction>() & *std::prev(along_wave<Direction>(path()));
        }

        template<class Iterator>
        auto targets_with_king(Iterator it) const
        {
                return targets_with_king<ray::direction_v<Iterator>>().test(*it);
        }

        template<int Direction>
        auto targets_with_pawn() const
        {
                return remaining_targets_ & *std::prev(along_wave<Direction>(path()));
        }

        template<class Iterator>
        auto targets_with_pawn(Iterator it) const
        {
                return targets_with_pawn<ray::direction_v<Iterator>>().test(*it);
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
                return set_intersection(path(), jump_start);
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

        auto greater_equal() const
        {
                static_assert(is_jump_precedence_v<Rules>, "");
                assert(is_totally_ordered(best_, current_));
                return current_ >= best_;
        }

        auto not_equal_to() const
        {
                static_assert(is_jump_precedence_v<Rules>, "");
                assert(greater_equal());
                return current_ != best_;
        }

        template<class Sequence>
        auto is_potential_duplicate(Sequence const& moves) const
        {
                return !moves.empty() && is_large();
        }

        auto is_large() const
        {
                return size() >= rules::large_jump<Rules>::value;
        }

        auto num_pieces() const
        {
                return static_cast<int>(removed_pieces_.size());
        }

        auto num_kings() const
        {
                return static_cast<int>(ordered_kings_.size());
        }

        auto ordered_kings() const
        {
                return ordered_kings_;
        }

        auto is_with_king() const
        {
                return is_with_king_;
        }

        auto is_promotion() const
        {
                return is_promotion_;
        }

//private:
        // modifiers

        // overload for apres-fini jump removal
        void capture_dispatch(int sq, std::false_type)
        {
                capture_impl(sq);
        }

        // overload for en-passant jump removal
        void capture_dispatch(int sq, std::true_type)
        {
                not_occupied_.set(sq);
                capture_impl(sq);
        }

        void capture_impl(int sq)
        {
                if (is_king(sq))
                        ordered_kings_.push_back(-num_pieces());
                removed_pieces_.push_back(sq);
                remaining_targets_.reset(sq);
        }

        // overload for apres-fini jump removal
        void release_dispatch(int sq, std::false_type)
        {
                release_impl(sq);
        }

        // overload for en-passant jump removal
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
                        ordered_kings_.pop_back();
        }

        void visit(int sq)
        {
                visited_path_.push_back(sq);
        }

        void leave()
        {
                visited_path_.pop_back();
        }

        // queries

        template<int Direction>
        auto remaining_targets() const
        {
        	// tag dispatching based on direction and king jump orthogonality
                return remaining_targets_dispatch(
                        std::integral_constant<
                                bool,
                                is_orthogonal(Angle{Direction}) &&
                                std::is_same< typename
                                        rules::orthogonality::king_jump<Rules>::type,
                                        rules::orthogonality::relative
                                >::value
                        >{}
                );
        }

        // overload for diagonal direction or king jump
        auto remaining_targets_dispatch(std::false_type) const
        {
                return remaining_targets_;
        }

        // overload for orthogonal direction and king jump
        auto remaining_targets_dispatch(std::true_type) const
        {
                return set_intersection(remaining_targets_, king_targets_);
        }

        auto size() const
        {
                return num_pieces();
        }

        // overload for pawn jumps without promotion
        template<bool Color>
        static auto is_promotion(int dest_sq, with::pawn)
        {
                return dctl::is_promotion<Color, Board>(dest_sq);
        }

        // overload for pawn jumps with an en-passant promotion
        template<bool Color>
        static auto is_promotion(int /* dest_sq */, with::king)
        {
                return true;
        }

        auto captured_kings(with::pawn) const
        {
                // tag dispatching on whether pawns can capture kings
                return captured_kings_dispatch(is_pawn_jump_king_t<Rules>{});
        }

        // overload for pawns that can capture kings
        auto captured_kings_dispatch(std::true_type) const
        {
                return captured_kings(with::king());
        }

        // overload for pawns that cannot capture kings
        auto captured_kings_dispatch(std::false_type) const
        {
                return Set{};
        }

        auto captured_kings(with::king) const
        {
                return set_intersection(captured_pieces(), king_targets_);
        }

        auto captured_pieces() const
        {
                return set_symmetric_difference(initial_targets_, remaining_targets_);
        }

        template<int Direction>
        static wave::Iterator<Board, Direction> along_wave(Set const& s)
        {
                return wave::make_iterator<Board, Direction>(s);
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

        auto last_piece() const
        {
                assert(!removed_pieces_.empty());
                return removed_pieces_.back();
        }

        // contracts

        auto invariant() const
        {
                return set_includes(initial_targets_, remaining_targets_);
        }

        // representation

        Set const king_targets_;
        Set initial_targets_;
        Set remaining_targets_;
        Set not_occupied_;
        Arena<int> sqa_;
        Arena<int> pca_;
        Arena<int> kca_;

        mutable stack_vector<int> visited_path_ = stack_vector<int>(Alloc<int>{sqa_});
        mutable stack_vector<int> removed_pieces_ = stack_vector<int>(Alloc<int>{pca_});
        mutable stack_vector<int> ordered_kings_ = stack_vector<int>(Alloc<int>{kca_});
        mutable bool is_with_king_{};
        mutable bool is_promotion_{};

        Value<Rules> current_{};
        Value<Rules> best_{};
};

}       // namespace successor
}       // namespace dctl
