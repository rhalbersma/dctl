#pragma once
#include <cassert>                                      // assert
#include <type_traits>                                  // false_type, true_type
#include <dctl/successor/copy/impl/primary_fwd.hpp>     // copy (primary template)
#include <dctl/successor/copy/impl/king_jumps.hpp>      // promote_en_passant
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/pieces/pawn.hpp>                         // pawn
#include <dctl/pieces/king.hpp>


#include <dctl/angle/degrees.hpp>
#include <dctl/angle/transform.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/board/iterator.hpp>                      // Increment, Prev
#include <dctl/node/promotion.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/bit/bitboard.hpp>                        // BitIndex
#include <dctl/utility/algorithm.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for pawn jumps generation
template<bool Color, class Position, class Sequence>
struct copy<Color, pieces::pawn, select::jumps, Position, Sequence>
{
        // enforce reference semantics
        copy(copy const&) = delete;
        copy& operator=(copy const&) = delete;

private:
        using KingJumps = copy<Color, pieces::king, select::jumps, Position, Sequence>;
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::jumps, Position>;

        // representation

        State& capture_;
        Sequence& moves_;

public:
        // structors

        explicit copy(State& c, Sequence& m)
        :
                capture_(c),
                moves_(m)
        {}

        // function call operators

        template<class Set>
        void operator()(Set const& active_pawns) const
        {
                // tag dispatching on whether pawns can capture kings
                if (active_pawns)
                        select_dispatch(active_pawns, rules::can_jump<Rules, pieces::pawn, pieces::king>());
        }

private:
        // overload for pawns that can capture kings
        template<class Set>
        void select_dispatch(Set const& active_pawns, std::true_type) const
        {
                branch(active_pawns);
        }

        // overload for pawns that cannot capture kings
        template<class Set>
        void select_dispatch(Set const& active_pawns, std::false_type) const
        {
                capture_.toggle_king_targets();
                branch(active_pawns);
                capture_.toggle_king_targets();
        }

        template<class Set>
        void branch(Set const& active_pawns) const
        {
                // tag dispatching on pawn jump directions
                branch_dispatch(active_pawns, rules::directions::pawn_jump<Rules>());
        }

        // overload for pawns that jump in the 8 diagonal and orthogonal directions
        template<class Set>
        void branch_dispatch(Set const& active_pawns, rules::directions::all) const
        {
                branch_dispatch(active_pawns, rules::directions::diag());
                branch_dispatch(active_pawns, rules::directions::orth());
        }

        // overload for pawns that jump in the 4 diagonal directions
        template<class Set>
        void branch_dispatch(Set const& active_pawns, rules::directions::diag) const
        {
                branch_dispatch(active_pawns, rules::directions::up  ());
                branch_dispatch(active_pawns, rules::directions::down());
        }

        // overload for pawns that jump in the 2 forward diagonal directions
        template<class Set>
        void branch_dispatch(Set const& active_pawns, rules::directions::up) const
        {
                serialize< Compass::left_up  >(active_pawns);
                serialize< Compass::right_up >(active_pawns);
        }

        // overload for pawns that jump in the 2 backward diagonal directions
        template<class Set>
        void branch_dispatch(Set const& active_pawns, rules::directions::down) const
        {
                serialize< Compass::left_down  >(active_pawns);
                serialize< Compass::right_down >(active_pawns);
        }

        // overload for pawns that jump in the 4 orthogonal directions
        template<class Set>
        void branch_dispatch(Set const& active_pawns, rules::directions::orth) const
        {
                serialize< Compass::left  >(active_pawns);
                serialize< Compass::right >(active_pawns);
                serialize< Compass::up    >(active_pawns);
                serialize< Compass::down  >(active_pawns);
        }

        template<int Direction, class Set>
        void serialize(Set const& active_pawns) const
        {
                for (auto sq: bit::bit_set<int, uint64_t, 1>(active_pawns & Prev<Board, Direction>()(capture_.template targets_with_pawn<Direction>())))
                        find(SquareIterator<Board, Direction>{sq});
        }

        template<class Direction>
        void find(Direction jumper) const
        {
                capture_.launch(jumper);
                find_first(jumper);
                capture_.finish(jumper);
        }

        template<class Direction>
        void find_first(Direction jumper) const
        {
                ++jumper;
                assert(bit::is_element(jumper, capture_.template targets_with_pawn<Direction>()));
                capture_.make(jumper);
                precedence(jumper);  // recursively find more jumps
                capture_.undo(jumper);
        }

        template<class Direction>
        void precedence(Direction jumper) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch(jumper, typename rules::is_precedence<Rules>());
        }

        // overload for no majority precedence
        template<class Direction>
        void precedence_dispatch(Direction jumper, std::false_type) const
        {
                ++jumper;
                if (!find_next(jumper))
                        add_pawn_jump(jumper);
        }

        // overload for majority precedence
        template<class Direction>
        void precedence_dispatch(Direction jumper, std::true_type) const
        {
                ++jumper;
                if (
                        !find_next(jumper) &&
                        capture_.greater_equal()
                ) {
                        if (capture_.not_equal_to()) {
                                capture_.improve();
                                moves_.clear();
                        }
                        add_pawn_jump(jumper);
                }
        }

        template<class Direction>
        bool find_next(Direction jumper) const
        {
                // tag dispatching on promotion condition
                return find_next_dispatch(jumper, rules::phase::promotion<Rules>());
        }

        // overload for pawns that promote apres-fini
        template<class Direction>
        bool find_next_dispatch(Diretion jumper, rules::phase::apres_fini) const
        {
                return find_next_impl(jumper);
        }

        // overload for pawns that promote en-passant
        template<class Direction>
        bool find_next_dispatch(Direction jumper, rules::phase::en_passant) const
        {
                return (!is_promotion_sq<Color, Board>(jumper)) ?
                        find_next_impl(jumper) :
                        promote_en_passant(jumper)
                ;
        }

        template<class Direction>
        bool find_next_impl(Direction jumper) const
        {
                return turn(jumper) | scan(jumper);
        }

        template<class Direction>
        bool promote_en_passant(Direction jumper) const
        {
                // tag dispatching on whether pawns can capture kings
                return promote_en_passant_dispatch(jumper, rules::can_jump<Rules, pieces::pawn, pieces::king>());
        }

        // overload for pawns that can capture kings
        template<class Direction>
        bool promote_en_passant_dispatch(Direction jumper, std::true_type) const
        {
                capture_.toggle_promotion();
                auto const found_next = KingJumps(capture_, moves_).promote_en_passant(jumper);
                capture_.toggle_promotion();
                return found_next;
        }

        // overload for pawns that cannot capture kings
        template<class Direction>
        bool promote_en_passant_dispatch(Direction jumper, std::false_type) const
        {
                capture_.toggle_promotion();    // no longer a pawn
                capture_.set_king_targets();    // can now capture kings
                auto const found_next = KingJumps(capture_, moves_).promote_en_passant(jumper);
                capture_.clear_king_targets();  // can no longer capture kings
                capture_.toggle_promotion();    // now a pawn again
                return found_next;
        }

        template<class Direction>
        bool turn(Direction jumper) const
        {
                // tag dispatching on pawn turn directions
                return turn_dispatch(jumper, rules::directions::pawn_turn<Rules>());
        }

        // overload for pawns that turn in all the 6 non-parallel diagonal and orthogonal directions
        template<class Direction>
        bool turn_dispatch(Direction jumper, rules::directions::all) const
        {
                return (
                        turn_dispatch(jumper, rules::directions::diag()) |
                        turn_dispatch(jumper, rules::directions::orth())
                );
        }

        // overload for pawns that turn in the 2 sideways directions
        template<class Direction>
        bool turn_dispatch(Direction jumper, rules::directions::diag) const
        {
                return (
                        scan(angle::rotate(jumper, angle::R090)) |
                        scan(angle::rotate(jumper, angle::L090))
                );
        }

        // overload for pawns that turn in the 1 mirrored forward direction
        template<class Direction>
        bool turn_dispatch(Direction jumper, rules::directions::up) const
        {
                return scan(angle::mirror(jumper, Compass::up));
        }

        // overload for pawns that turn in the 1 mirrored backward direction
        template<class Direction>
        bool turn_dispatch(Direction jumper, rules::directions::down) const
        {
                return scan(angle::mirror(jumper, Compass::down));
        }

        // overload for pawns that turn in the remaining 4 diagonal or orthogonal directions
        template<class Direction>
        bool turn_dispatch(Direction jumper, rules::directions::orth) const
        {
                return (
                        scan(angle::rotate(jumper, angle::R045)) |
                        scan(angle::rotate(jumper, angle::L045)) |
                        scan(angle::rotate(jumper, angle::R135)) |
                        scan(angle::rotate(jumper, angle::L135))
                );
        }

        template<class Direction>
        bool scan(Direction jumper) const
        {
                ++jumper;
                return jump(jumper);
        }

        template<class Direction>
        bool jump(Direction jumper) const
        {
                if (!bit::is_element(jumper, capture_.template targets_with_pawn<Direction>()))
                        return false;

                capture_.make(jumper);
                precedence(jumper);  // recursively find more jumps
                capture_.undo(jumper);
                return true;
        }

        template<class Square>
        void add_pawn_jump(Square dest_sq) const
        {
                // tag dispatching on ambiguity of pawn jumps
                add_pawn_jump_dispatch(dest_sq, rules::is_unambiguous_pawn_jump<Rules>());
        }

        // overload for pawn jumps that are always unambiguous
        template<class Square>
        void add_pawn_jump_dispatch(Square dest_sq, std::true_type) const
        {
                capture_.template add_pawn_jump<Color, with::pawn>(dest_sq, moves_);
        }

        // overload for pawn jumps that are potentially ambiguous
        template<class Square>
        void add_pawn_jump_dispatch(Square dest_sq, std::false_type) const
        {
                auto const check_duplicate = rules::is_remove_duplicates<Rules>::value && capture_.is_potential_duplicate(moves_);
                capture_.template add_pawn_jump<Color, with::pawn>(dest_sq, moves_);
                if (check_duplicate && util::is_duplicate_back(moves_))
                        moves_.pop_back();
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
