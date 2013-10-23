#pragma once
#include <cassert>                                      // assert
#include <type_traits>                                  // false_type, true_type
#include <dctl/successor/copy/impl/primary_fwd.hpp>     // copy (primary template)
#include <dctl/successor/propagate/jumps.hpp>           // Propagate
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/pieces/king.hpp>                         // king

#include <dctl/angle/degrees.hpp>                       // Degrees
#include <dctl/angle/transform.hpp>                     // rotate
#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/board/iterator.hpp>                      // Increment, Next, Prev
#include <dctl/rules/traits.hpp>                        // traits
#include <dctl/bit/bitboard.hpp>                        // BitIndex
#include <dctl/utility/algorithm.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for king jumps generation
template<bool Color, class Position, class Sequence>
struct copy<Color, pieces::king, select::jumps, Position, Sequence>
{
        // enforce reference semantics
        copy(copy const&) = delete;
        copy& operator=(copy const&) = delete;

private:
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Move = typename Sequence::value_type;
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
        void operator()(Set const& active_kings) const
        {
                // tag dispatching on relative king jump precedence
                if (active_kings)
                        select_dispatch(active_kings, rules::precedence::is_relative_king<Rules>());
        }

        template<int Direction>
        bool promote_en_passant(BitIndex jumper) const
        {
                assert((is_promotion_sq<Color, Board>(jumper)));
                return find_next<Direction>(jumper);
        }

private:
        // overload for no relative king jump precedence
        template<class Set>
        void select_dispatch(Set const& active_kings, std::false_type) const
        {
                serialize(active_kings);
        }

        // overload for relative king jump precedence
        template<class Set>
        void select_dispatch(Set const& active_kings, std::true_type) const
        {
                capture_.toggle_with_king();
                serialize(active_kings);
                capture_.toggle_with_king();
        }

        template<class Set>
        void serialize(Set const& active_kings) const
        {
                assert(!bit::empty(active_kings));
                for (auto sq: bit::bit_set<int, uint64_t, 1>(active_kings))
                        find(BitBoard{1} << sq);
        }

        void find(BitIndex jumper) const
        {
                capture_.launch(jumper);
                branch(jumper);
                capture_.finish(jumper);
        }

        void branch(BitIndex jumper) const
        {
                // tag dispatching on king jump directions
                branch_dispatch(jumper, rules::directions::king_jump<Rules>());
        }

        // overload for kings that jump in the 8 diagonal and orthogonal directions
        void branch_dispatch(BitIndex jumper, rules::directions::all) const
        {
                branch_dispatch(jumper, rules::directions::diag());
                branch_dispatch(jumper, rules::directions::orth());
        }

        // overload for kings that jump in the 4 diagonal directions
        void branch_dispatch(BitIndex jumper, rules::directions::diag) const
        {
                find_first< Compass::left_up    >(jumper);
                find_first< Compass::right_up   >(jumper);
                find_first< Compass::left_down  >(jumper);
                find_first< Compass::right_down >(jumper);
        }

        // overload for kings that jump in the 4 orthogonal directions
        void branch_dispatch(BitIndex jumper, rules::directions::orth) const
        {
                find_first< Compass::left  >(jumper);
                find_first< Compass::right >(jumper);
                find_first< Compass::up    >(jumper);
                find_first< Compass::down  >(jumper);
        }

        template<int Direction>
        void find_first(BitIndex jumper) const
        {
                slide<Direction>(jumper, capture_.template path<Direction>());
                if (bit::is_element(jumper, capture_.template targets_with_king<Direction>())) {
                        capture_.make(jumper);
                        precedence<Direction>(jumper);  // recursively find more jumps
                        capture_.undo(jumper);
                }
        }

        template<int Direction>
        void precedence(BitIndex jumper) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch<Direction>(jumper, rules::is_precedence<Rules>());
        }

        // overload for no majority precedence
        template<int Direction>
        void precedence_dispatch(BitIndex jumper, std::false_type) const
        {
                Increment<Board, Direction>()(jumper);
                if (!find_next<Direction>(jumper))
                        add_king_jump<Direction>(jumper);
        }

        // overload for majority precedence
        template<int Direction>
        void precedence_dispatch(BitIndex jumper, std::true_type) const
        {
                Increment<Board, Direction>()(jumper);
                if (
                        !find_next<Direction>(jumper) &&
                        capture_.greater_equal()
                ) {
                        if (capture_.not_equal_to()) {
                                capture_.improve();
                                moves_.clear();
                        }
                        add_king_jump<Direction>(jumper);
                }
        }

        template<int Direction>
        bool find_next(BitIndex jumper) const
        {
                // tag dispatching on king jump direction reversal
                return find_next_dispatch<Direction>(jumper, rules::directions::is_reversal<Rules>());
        }

        // overload for kings that cannot reverse their capture direction
        template<int Direction>
        bool find_next_dispatch(BitIndex jumper, std::false_type) const
        {
                return land<Direction>(jumper);
        }

        // overload for kings that can reverse their capture direction
        template<int Direction>
        bool find_next_dispatch(BitIndex jumper, std::true_type) const
        {
                return land<Direction>(jumper) | reverse<Direction>(jumper);
        }

        template<int Direction>
        bool reverse(BitIndex jumper) const
        {
                return scan< angle::rotate(Direction, angle::D180) >(jumper);
        }

        template<int Direction>
        bool land(BitIndex jumper) const
        {
                // tag dispatching on king jump landing range after intermediate captures
                return land_dispatch<Direction>(jumper, rules::range::land<Rules>());
        }

        // overload for kings that land immediately if the intermediate capture is a king, and slide through otherwise
        template<int Direction>
        void land_dispatch(BitIndex jumper, rules::range::distance_1K) const
        {
                if (capture_.is_king(Prev<Board, Direction>()(jumper)))
                        land_dispatch<Direction>(jumper, rules::range::distance_1());
                else
                        land_dispatch<Direction>(jumper, rules::range::distance_N());
        }

        // overload for kings that can only land on the immediately adjacent square
        template<int Direction>
        bool land_dispatch(BitIndex jumper, rules::range::distance_1) const
        {
                return turn<Direction>(jumper) | scan<Direction>(jumper);
        }

        // overload for kings that can land on any square along the current direction
        template<int Direction>
        bool land_dispatch(BitIndex jumper, rules::range::distance_N) const
        {
                // NOTE: capture_.template path<Direction>() would be an ERROR here
                // because we need all landing squares rather than the directional launching squares subset
                assert(bit::is_element(jumper, capture_.path()));
                auto found_next = false;
                do {
                        found_next |= turn<Direction>(jumper);
                        Increment<Board, Direction>()(jumper);
                } while (bit::is_element(jumper, capture_.path()));
                return found_next |= jump<Direction>(jumper);
        }

        template<int Direction>
        bool turn(BitIndex jumper) const
        {
                // tag dispatching on king turn directions
                return turn_dispatch<Direction>(jumper, rules::directions::king_turn<Rules>());
        }

        // overload for kings that turn in all the 6 non-parallel diagonal and orthogonal directions
        template<int Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::all) const
        {
                return (
                        turn_dispatch<Direction>(jumper, rules::directions::diag()) |
                        turn_dispatch<Direction>(jumper, rules::directions::orth())
                );
        }

        // overload for kings that turn in the 2 sideways directions
        template<int Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::diag) const
        {
                return (
                        scan< angle::rotate(Direction, angle::R090) >(jumper) |
                        scan< angle::rotate(Direction, angle::L090) >(jumper)
                );
        }

        // overload for kings that turn in the remaining 4 diagonal or orthogonal directions
        template<int Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::orth) const
        {
                return (
                        scan< angle::rotate(Direction, angle::R045) >(jumper) |
                        scan< angle::rotate(Direction, angle::L045) >(jumper) |
                        scan< angle::rotate(Direction, angle::R135) >(jumper) |
                        scan< angle::rotate(Direction, angle::L135) >(jumper)
                );
        }

        template<int Direction>
        bool scan(BitIndex jumper) const
        {
                slide<Direction>(jumper, capture_.template path<Direction>());
                return jump<Direction>(jumper);
        }

        template<int Direction, class Set>
        void slide(BitIndex& jumper, Set const& path) const
        {
                // tag dispatching on king range
                slide_dispatch<Direction>(jumper, path, rules::range::scan<Rules>());
        }

        // overload for short ranged kings
        template<int Direction, class Set>
        void slide_dispatch(BitIndex& jumper, Set const& /* path */, rules::range::distance_1) const
        {
                Increment<Board, Direction>()(jumper);
        }

        // overload for long ranged kings
        template<int Direction, class Set>
        void slide_dispatch(BitIndex& jumper, Set const& path, rules::range::distance_N) const
        {
                do Increment<Board, Direction>()(jumper); while (bit::is_element(jumper, path));
        }

        template<int Direction>
        bool jump(BitIndex jumper) const
        {
                if (!bit::is_element(jumper, capture_.template targets_with_king<Direction>()))
                        return false;

                capture_.make(jumper);
                precedence<Direction>(jumper);  // recursively find more jumps
                capture_.undo(jumper);
                return true;
        }

        template<int Direction>
        void add_king_jump(BitIndex dest_sq) const
        {
                auto const check_duplicate = rules::is_remove_duplicates<Rules>::value && capture_.is_potential_duplicate(moves_);

                // tag dispatching on king halt after final capture
                add_king_jump_dispatch<Direction>(dest_sq, check_duplicate, rules::range::halt<Rules>());
        }

        // overload for kings that halt immediately if the final capture is a king, and slide through otherwise
        template<int Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, bool check_duplicate, rules::range::distance_1K) const
        {
                if (capture_.is_king(Prev<Board, Direction>()(dest_sq)))
                        add_king_jump_dispatch<Direction>(dest_sq, check_duplicate, rules::range::distance_1());
                else
                        add_king_jump_dispatch<Direction>(dest_sq, check_duplicate, rules::range::distance_N());
        }

        // overload for kings that halt immediately after the final capture
        template<int Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, bool check_duplicate, rules::range::distance_1) const
        {
                add_king_jump(dest_sq, check_duplicate);
        }

        // overload for kings that slide through after the final capture
        template<int Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, bool check_duplicate, rules::range::distance_N) const
        {
                // NOTE: capture_.template path<Direction>() would be an ERROR here
                // because we need all halting squares rather than the directional launching squares subset
                assert(bit::is_element(dest_sq, capture_.path()));
                do {
                        add_king_jump(dest_sq, check_duplicate);
                        Increment<Board, Direction>()(dest_sq);
                } while (bit::is_element(dest_sq, capture_.path()));
        }

        void add_king_jump(BitIndex dest_sq, bool check_duplicate) const
        {
                // tag dispatching on promotion condition
                add_king_jump_dispatch(dest_sq, rules::phase::promotion<Rules>());
                if (check_duplicate && util::is_duplicate_back(moves_))
                        moves_.pop_back();
        }

        // overload for pawns that promote apres-fini
        void add_king_jump_dispatch(BitIndex dest_sq, rules::phase::apres_fini) const
        {
                capture_.template add_king_jump<Color>(dest_sq, moves_);
        }

        // overload for pawns that promote en-passant
        void add_king_jump_dispatch(BitIndex dest_sq, rules::phase::en_passant) const
        {
                if (!capture_.is_promotion())
                        capture_.template add_king_jump<Color>(dest_sq, moves_);
                else
                        capture_.template add_pawn_jump<Color, with::king>(dest_sq, moves_);
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
