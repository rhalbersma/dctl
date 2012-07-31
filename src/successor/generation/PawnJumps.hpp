#pragma once
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
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
        void operator()(Position const& p, Stack& moves)
        {
                if (auto const active_pawns = p.pawns(Color)) {
                        State capture(p, moves);
                        select(active_pawns, moves);
                }
        }

        void operator()(Position const& p, State& capture)
        {
                if (auto const active_pawns = p.pawns(Color))
                        select(active_pawns, capture);
        }

private:
        void select(BitBoard active_pawns, State& capture)
        {
                // tag dispatching on whether pawns can capture kings
                select_dispatch(active_pawns, capture, typename Rules::is_pawns_jump_kings());
        }

        // overload for pawns that can capture kings
        void select_dispatch(BitBoard active_pawns, State& capture, boost::mpl::true_)
        {
                branch(active_pawns, capture);
        }

        // overload for pawns that cannot capture kings
        void select_dispatch(BitBoard active_pawns, State& capture, boost::mpl::false_)
        {
                capture.toggle_king_targets();
                branch(active_pawns, capture);
                capture.toggle_king_targets();
        }    

        void branch(BitBoard active_pawns, State& capture)
        {
                // tag dispatching on pawn capture directions
                branch_dispatch(active_pawns, capture, typename Rules::pawn_jump_directions());
        }

        // overload for pawns that capture in the 8 diagonal and orthogonal directions
        void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::all)
        {
                branch_dispatch(active_pawns, capture, rules::directions::diag());
                branch_dispatch(active_pawns, capture, rules::directions::orth());
        }

        // overload for pawns that capture in the 4 diagonal directions
        void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::diag)
        {
                branch_dispatch(active_pawns, capture, rules::directions::up  ());
                branch_dispatch(active_pawns, capture, rules::directions::down());
        }

        // overload for pawns that capture in the 2 forward diagonal directions
        void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::up)
        {
                serialize<typename Compass::left_up >(active_pawns, capture);
                serialize<typename Compass::right_up>(active_pawns, capture);
        }

        // overload for pawns that capture in the 2 backward diagonal directions
        void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::down)
        {
                serialize<typename Compass::left_down >(active_pawns, capture);
                serialize<typename Compass::right_down>(active_pawns, capture);
        }

        // overload for pawns that capture in the 4 orthogonal directions
        void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::orth)
        {
                serialize<typename Compass::left >(active_pawns, capture);
                serialize<typename Compass::right>(active_pawns, capture);
                serialize<typename Compass::up   >(active_pawns, capture);
                serialize<typename Compass::down >(active_pawns, capture);
        }

        template<typename Direction>
        void serialize(BitBoard active_pawns, State& capture)
        {
                BitIndex jumper;
                for (
                        active_pawns &= Pull<Board, Direction>()(capture.template targets<Direction>());
                        active_pawns;
                        bit::clear_first(active_pawns)
                ) {
                        jumper = bit::get_first(active_pawns);
                        capture.launch(jumper);
                        find_first<Direction>(jumper, capture);
                        capture.finish(jumper);
                }
        }

        template<typename Direction>
        void find_first(BitBoard jumper, State& capture)
        {
                Board::advance<Direction>(jumper);
                BOOST_ASSERT(bit::is_element(jumper, capture.template targets<Direction>()));
                capture.make(jumper);
                add_jump<Direction>(jumper, capture);   // recursively find more jumps
                capture.undo(jumper);
        }

        template<typename Direction>
        void add_jump(BitIndex jumper, State& capture)
        {
                Board::advance<Direction>(jumper);
                if (
                        !find_next<Direction>(jumper, capture) &&
                        capture.greater_equal()
                ) {
                        if (capture.not_equal_to())
                                capture.improve();
                        capture.template add_pawn_jump<Color>(jumper);
                }
        }

        template<typename Direction>
        bool find_next(BitIndex jumper, State& capture)
        {
                // tag dispatching on promotion condition
                return find_next_dispatch<Direction>(jumper, capture, typename Rules::pawn_promotion());
        }

        // overload for pawns that promote apres-fini
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, State& capture, rules::promotion::apres_fini)
        {
                return find_next_impl<Direction>(jumper, capture);
        }

        // overload for pawns that promote en-passant
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, State& capture, rules::promotion::en_passant)
        {
                if (!is_promotion_sq<Color, Board>(jumper))
                        return find_next_impl<Direction>(jumper, capture);
                else
                        return promote_en_passant<Direction>(jumper, capture);
        }

        template<typename Direction>
        bool promote_en_passant(BitIndex jumper, State& capture)
        {
                // tag dispatching on whether pawns can capture kings
                return promote_en_passant_dispatch<Direction>(jumper, capture, typename Rules::is_pawns_jump_kings());
        }

        // overload for pawns that can capture kings
        template<typename Direction>
        bool promote_en_passant_dispatch(BitIndex jumper, State& capture, boost::mpl::true_)
        {
                capture.toggle_promotion();
                auto const found_next = KingJumps().template promote_en_passant<Direction>(jumper, capture);
                capture.toggle_promotion();
                return found_next;
        }

        // overload for pawns that cannot capture kings
        template<typename Direction>
        bool promote_en_passant_dispatch(BitIndex jumper, State& capture, boost::mpl::false_)
        {
                capture.toggle_promotion();     // no longer a pawn
                capture.toggle_king_targets();  // can now capture kings
                auto const found_next = KingJumps().template promote_en_passant<Direction>(jumper, capture);
                capture.toggle_king_targets();  // can no longer capture kings
                capture.toggle_promotion();     // now a pawn again
                return found_next;
        } 

        template<typename Direction>
        bool find_next_impl(BitIndex jumper, State& capture)
        {
                return (
                        turn<Direction>(jumper, capture) |
                        scan<Direction>(jumper, capture)
                );
        }

        template<typename Direction>
        bool turn(BitIndex jumper, State& capture)
        {
                // tag dispatching on man turn directions
                return turn_dispatch<Direction>(jumper, capture, typename Rules::pawn_turn_directions());
        }

        // overload for turns in all the 6 non-parallel diagonal and orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::all)
        {
                return (
                        turn_dispatch<Direction>(jumper, capture, rules::directions::diag()) |
                        turn_dispatch<Direction>(jumper, capture, rules::directions::orth())
                );
        }

        // overload for turns in the 2 sideways directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::diag)
        {
                return (
                        scan< typename rotate< Direction, angle::R090 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L090 >::type >(jumper, capture)
                );
        }

        // overload for turns in the 1 mirrored forward direction
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::up)
        {
                return scan< typename mirror< Direction, typename Compass::up >::type >(jumper, capture);
        }

        // overload for turns in the 1 mirrored backward direction
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::down)
        {
                return scan< typename mirror< Direction, typename Compass::down >::type >(jumper, capture);
        }

        // overload for turns in the remaining 4 diagonal or orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::orth)
        {
                return (
                        scan< typename rotate< Direction, angle::R045 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L045 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::R135 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L135 >::type >(jumper, capture)
                );
        }

        template<typename Direction>
        bool scan(BitIndex jumper, State& capture)
        {
                Board::advance<Direction>(jumper);
                return jump<Direction>(jumper, capture);
        }

        template<typename Direction>
        bool jump(BitIndex jumper, State& capture)
        {
                if (!bit::is_element(jumper, capture.template targets<Direction>())) 
                        return false;                   // terminated

                capture.make(jumper);
                add_jump<Direction>(jumper, capture);   // recursively find more jumps
                capture.undo(jumper);                
                return true;                            // not terminated 
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
