#pragma once
#include <boost/mpl/bool.hpp>           // false_, true_
#include "Generator_fwd.hpp"
#include "KingJumps.hpp"                // promote_en_passant
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Degrees.hpp"
#include "../../board/Shift.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn jumps generation
template<bool Color, typename Position>
struct generator<Color, Material::pawn, Jumps, Position>
{
private:
        // typedefs

        typedef generator<Color, Material::king, Jumps, Position> KingJumps;
        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;
        typedef capture::State<Position> State;

public:
        void operator()(Position const& p, Stack& moves) const
        {
                if (auto const active_pawns = p.pawns(Color)) {
                        State capture(p, moves);
                        select(active_pawns, moves);
                }
        }

        void operator()(Position const& p, State& capture) const
        {
                if (auto const active_pawns = p.pawns(Color))
                        select(active_pawns, capture);
        }

private:
        void select(BitBoard active_pawns, State& capture) const
        {
                // tag dispatching on whether pawns can capture kings
                select_dispatch(active_pawns, capture, typename Rules::is_pawns_jump_kings());
        }

        // overload for pawns that can capture kings
        void select_dispatch(BitBoard active_pawns, State& capture, boost::mpl::true_) const
        {
                branch(active_pawns, capture);
        }

        // overload for pawns that cannot capture kings
        void select_dispatch(BitBoard active_pawns, State& capture, boost::mpl::false_) const
        {
                capture.toggle_king_targets();
                branch(active_pawns, capture);
                capture.toggle_king_targets();
        }

        void branch(BitBoard active_pawns, State& capture) const
        {
                // tag dispatching on pawn jump directions
                branch_dispatch(active_pawns, capture, typename Rules::pawn_jump_directions());
        }

        // overload for pawns that capture in the 8 diagonal and orthogonal directions
        void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::all) const
        {
                branch_dispatch(active_pawns, capture, rules::directions::diag());
                branch_dispatch(active_pawns, capture, rules::directions::orth());
        }

        // overload for pawns that capture in the 4 diagonal directions
        void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::diag) const
        {
                branch_dispatch(active_pawns, capture, rules::directions::up  ());
                branch_dispatch(active_pawns, capture, rules::directions::down());
        }

        // overload for pawns that capture in the 2 forward diagonal directions
        void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::up) const
        {
                serialize<typename Compass::left_up >(active_pawns, capture);
                serialize<typename Compass::right_up>(active_pawns, capture);
        }

        // overload for pawns that capture in the 2 backward diagonal directions
        void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::down) const
        {
                serialize<typename Compass::left_down >(active_pawns, capture);
                serialize<typename Compass::right_down>(active_pawns, capture);
        }

        // overload for pawns that capture in the 4 orthogonal directions
        void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::orth) const
        {
                serialize<typename Compass::left >(active_pawns, capture);
                serialize<typename Compass::right>(active_pawns, capture);
                serialize<typename Compass::up   >(active_pawns, capture);
                serialize<typename Compass::down >(active_pawns, capture);
        }

        template<typename Direction>
        void serialize(BitBoard active_pawns, State& capture) const
        {
                for (
                        active_pawns &= Pull<Board, Direction>()(capture.template targets_with_pawn<Direction>());
                        active_pawns;
                        bit::clear_first(active_pawns)
                )
                        find<Direction>(bit::get_first(active_pawns), capture);
        }

        template<typename Direction>
        void find(BitIndex jumper, State& capture) const
        {
                capture.launch(jumper);
                find_first<Direction>(jumper, capture);
                capture.finish(jumper);
        }

        template<typename Direction>
        void find_first(BitIndex jumper, State& capture) const
        {
                Board::advance<Direction>(jumper);
                BOOST_ASSERT(bit::is_element(jumper, capture.template targets_with_pawn<Direction>()));
                capture.make(jumper);
                precedence<Direction>(jumper, capture); // recursively find more jumps
                capture.undo(jumper);
        }

        template<typename Direction>
        void precedence(BitIndex jumper, State& capture) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch<Direction>(jumper, capture, typename Rules::is_precedence());
        }

        // overload for no majority precedence
        template<typename Direction>
        void precedence_dispatch(BitIndex jumper, State& capture, boost::mpl::false_) const
        {
                Board::advance<Direction>(jumper);
                if (!find_next<Direction>(jumper, capture))
                        add_pawn_jump(jumper, capture);
        }

        // overload for majority precedence
        template<typename Direction>
        void precedence_dispatch(BitIndex jumper, State& capture, boost::mpl::true_) const
        {
                Board::advance<Direction>(jumper);
                if (
                        !find_next<Direction>(jumper, capture) &&
                        capture.greater_equal()
                ) {
                        if (capture.not_equal_to())
                                capture.improve();
                        add_pawn_jump(jumper, capture);
                }
        }

        template<typename Direction>
        bool find_next(BitIndex jumper, State& capture) const
        {
                // tag dispatching on promotion condition
                return find_next_dispatch<Direction>(jumper, capture, typename Rules::pawn_promotion());
        }

        // overload for pawns that promote apres-fini
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, State& capture, rules::promotion::apres_fini) const
        {
                return find_next_impl<Direction>(jumper, capture);
        }

        // overload for pawns that promote en-passant
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, State& capture, rules::promotion::en_passant) const
        {
                if (!is_promotion_sq<Color, Board>(jumper))
                        return find_next_impl<Direction>(jumper, capture);
                else
                        return promote_en_passant<Direction>(jumper, capture);
        }

        template<typename Direction>
        bool promote_en_passant(BitIndex jumper, State& capture) const
        {
                // tag dispatching on whether pawns can capture kings
                return promote_en_passant_dispatch<Direction>(jumper, capture, typename Rules::is_pawns_jump_kings());
        }

        // overload for pawns that can capture kings
        template<typename Direction>
        bool promote_en_passant_dispatch(BitIndex jumper, State& capture, boost::mpl::true_) const
        {
                capture.toggle_promotion();
                auto const found_next = KingJumps().template promote_en_passant<Direction>(jumper, capture);
                capture.toggle_promotion();
                return found_next;
        }

        // overload for pawns that cannot capture kings
        template<typename Direction>
        bool promote_en_passant_dispatch(BitIndex jumper, State& capture, boost::mpl::false_) const
        {
                capture.toggle_promotion();     // no longer a pawn
                capture.toggle_king_targets();  // can now capture kings
                auto const found_next = KingJumps().template promote_en_passant<Direction>(jumper, capture);
                capture.toggle_king_targets();  // can no longer capture kings
                capture.toggle_promotion();     // now a pawn again
                return found_next;
        }

        template<typename Direction>
        bool find_next_impl(BitIndex jumper, State& capture) const
        {
                return (
                        turn<Direction>(jumper, capture) |
                        scan<Direction>(jumper, capture)
                );
        }

        template<typename Direction>
        bool turn(BitIndex jumper, State& capture) const
        {
                // tag dispatching on man turn directions
                return turn_dispatch<Direction>(jumper, capture, typename Rules::pawn_turn_directions());
        }

        // overload for turns in all the 6 non-parallel diagonal and orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::all) const
        {
                return (
                        turn_dispatch<Direction>(jumper, capture, rules::directions::diag()) |
                        turn_dispatch<Direction>(jumper, capture, rules::directions::orth())
                );
        }

        // overload for turns in the 2 sideways directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::diag) const
        {
                return (
                        scan< typename rotate< Direction, angle::R090 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L090 >::type >(jumper, capture)
                );
        }

        // overload for turns in the 1 mirrored forward direction
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::up) const
        {
                return scan< typename mirror< Direction, typename Compass::up >::type >(jumper, capture);
        }

        // overload for turns in the 1 mirrored backward direction
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::down) const
        {
                return scan< typename mirror< Direction, typename Compass::down >::type >(jumper, capture);
        }

        // overload for turns in the remaining 4 diagonal or orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::orth) const
        {
                return (
                        scan< typename rotate< Direction, angle::R045 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L045 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::R135 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L135 >::type >(jumper, capture)
                );
        }

        template<typename Direction>
        bool scan(BitIndex jumper, State& capture) const
        {
                Board::advance<Direction>(jumper);
                return jump<Direction>(jumper, capture);
        }

        template<typename Direction>
        bool jump(BitIndex jumper, State& capture) const
        {
                if (!bit::is_element(jumper, capture.template targets_with_pawn<Direction>()))
                        return false;

                capture.make(jumper);
                precedence<Direction>(jumper, capture); // recursively find more jumps
                capture.undo(jumper);
                return true;
        }

        void add_pawn_jump(BitIndex dest_sq, State& capture) const
        {
                // tag dispatching on ambiguity of pawn jumps
                add_pawn_jump_dispatch(dest_sq, capture, typename Rules::is_unambiguous_pawn_jump());
        }

        // overload for pawn jumps that are always unambiguous
        void add_pawn_jump_dispatch(BitIndex dest_sq, State& capture, boost::mpl::true_) const
        {
                capture.template add_pawn_jump<Color, capture::with::pawn>(dest_sq);
        }

        // overload for pawn jumps that are potentially ambiguous
        void add_pawn_jump_dispatch(BitIndex dest_sq, State& capture, boost::mpl::false_) const
        {
                auto const ambiguous = rules::is_check_jump_uniqueness<Rules>::value && capture.is_ambiguous();
                capture.template add_pawn_jump<Color, capture::with::pawn>(dest_sq);
                if (ambiguous)
                        capture.remove_non_unique_back();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
