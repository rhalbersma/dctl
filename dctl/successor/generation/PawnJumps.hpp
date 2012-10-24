#pragma once
#include <functional>                   // function
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/mpl/bool.hpp>           // false_, true_
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/generation/Generator_fwd.hpp>
#include <dctl/successor/generation/KingJumps.hpp>                // promote_en_passant
#include <dctl/successor/Select.hpp>
#include <dctl/angle/Degrees.hpp>
#include <dctl/bit/Bit.hpp>
#include <dctl/board/Compass.hpp>
#include <dctl/board/Shift.hpp>
#include <dctl/capture/State.hpp>
#include <dctl/mpl/transform.hpp>
#include <dctl/node/Material.hpp>
#include <dctl/node/Promotion.hpp>
#include <dctl/rules/Enum.hpp>
#include <dctl/utility/IntegerTypes.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn jumps generation
template<bool Color, typename Position>
struct generator<Color, Material::pawn, Jumps, Position>
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&)>
{
private:
        // typedefs

        typedef generator<Color, Material::king, Jumps, Position> KingJumps;
        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef Compass<Color, Board> Compass;
        typedef capture::State<Position> State;

        // representation

        State& capture_;

public:
        // structors

        explicit generator(State& c)
        : 
                capture_(c) 
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                if (auto const active_pawns = p.pawns(Color))
                        select(active_pawns);
        }

private:
        void select(BitBoard active_pawns) const
        {
                // tag dispatching on whether pawns can capture kings
                select_dispatch(active_pawns, typename Rules::is_pawns_jump_kings());
        }

        // overload for pawns that can capture kings
        void select_dispatch(BitBoard active_pawns, boost::mpl::true_) const
        {
                branch(active_pawns);
        }

        // overload for pawns that cannot capture kings
        void select_dispatch(BitBoard active_pawns, boost::mpl::false_) const
        {
                capture_.toggle_king_targets();
                branch(active_pawns);
                capture_.toggle_king_targets();
        }

        void branch(BitBoard active_pawns) const
        {
                // tag dispatching on pawn jump directions
                branch_dispatch(active_pawns, typename Rules::pawn_jump_directions());
        }

        // overload for pawns that capture in the 8 diagonal and orthogonal directions
        void branch_dispatch(BitBoard active_pawns, rules::directions::all) const
        {
                branch_dispatch(active_pawns, rules::directions::diag());
                branch_dispatch(active_pawns, rules::directions::orth());
        }

        // overload for pawns that capture in the 4 diagonal directions
        void branch_dispatch(BitBoard active_pawns, rules::directions::diag) const
        {
                branch_dispatch(active_pawns, rules::directions::up  ());
                branch_dispatch(active_pawns, rules::directions::down());
        }

        // overload for pawns that capture in the 2 forward diagonal directions
        void branch_dispatch(BitBoard active_pawns, rules::directions::up) const
        {
                serialize<typename Compass::left_up >(active_pawns);
                serialize<typename Compass::right_up>(active_pawns);
        }

        // overload for pawns that capture in the 2 backward diagonal directions
        void branch_dispatch(BitBoard active_pawns, rules::directions::down) const
        {
                serialize<typename Compass::left_down >(active_pawns);
                serialize<typename Compass::right_down>(active_pawns);
        }

        // overload for pawns that capture in the 4 orthogonal directions
        void branch_dispatch(BitBoard active_pawns, rules::directions::orth) const
        {
                serialize<typename Compass::left >(active_pawns);
                serialize<typename Compass::right>(active_pawns);
                serialize<typename Compass::up   >(active_pawns);
                serialize<typename Compass::down >(active_pawns);
        }

        template<typename Direction>
        void serialize(BitBoard active_pawns) const
        {
                for (
                        active_pawns &= Prev<Board, Direction>()(capture_.template targets_with_pawn<Direction>());
                        active_pawns;
                        bit::first::clear(active_pawns)
                )
                        find<Direction>(bit::first::equal(active_pawns));
        }

        template<typename Direction>
        void find(BitIndex jumper) const
        {
                capture_.launch(jumper);
                find_first<Direction>(jumper);
                capture_.finish(jumper);
        }

        template<typename Direction>
        void find_first(BitIndex jumper) const
        {
                Advance<Board, Direction>()(jumper);
                BOOST_ASSERT(bit::is_element(jumper, capture_.template targets_with_pawn<Direction>()));
                capture_.make(jumper);
                precedence<Direction>(jumper); // recursively find more jumps
                capture_.undo(jumper);
        }

        template<typename Direction>
        void precedence(BitIndex jumper) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch<Direction>(jumper, typename Rules::is_precedence());
        }

        // overload for no majority precedence
        template<typename Direction>
        void precedence_dispatch(BitIndex jumper, boost::mpl::false_) const
        {
                Advance<Board, Direction>()(jumper);
                if (!find_next<Direction>(jumper))
                        add_pawn_jump(jumper);
        }

        // overload for majority precedence
        template<typename Direction>
        void precedence_dispatch(BitIndex jumper, boost::mpl::true_) const
        {
                Advance<Board, Direction>()(jumper);
                if (
                        !find_next<Direction>(jumper) &&
                        capture_.greater_equal()
                ) {
                        if (capture_.not_equal_to())
                                capture_.improve();
                        add_pawn_jump(jumper);
                }
        }

        template<typename Direction>
        bool find_next(BitIndex jumper) const
        {
                // tag dispatching on promotion condition
                return find_next_dispatch<Direction>(jumper, typename Rules::pawn_promotion());
        }

        // overload for pawns that promote apres-fini
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, rules::promotion::apres_fini) const
        {
                return find_next_impl<Direction>(jumper);
        }

        // overload for pawns that promote en-passant
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, rules::promotion::en_passant) const
        {
                if (!is_promotion_sq<Color, Board>(jumper))
                        return find_next_impl<Direction>(jumper);
                else
                        return promote_en_passant<Direction>(jumper);
        }

        template<typename Direction>
        bool promote_en_passant(BitIndex jumper) const
        {
                // tag dispatching on whether pawns can capture kings
                return promote_en_passant_dispatch<Direction>(jumper, typename Rules::is_pawns_jump_kings());
        }

        // overload for pawns that can capture kings
        template<typename Direction>
        bool promote_en_passant_dispatch(BitIndex jumper, boost::mpl::true_) const
        {
                capture_.toggle_promotion();
                auto const found_next = KingJumps(capture_).template promote_en_passant<Direction>(jumper);
                capture_.toggle_promotion();
                return found_next;
        }

        // overload for pawns that cannot capture kings
        template<typename Direction>
        bool promote_en_passant_dispatch(BitIndex jumper, boost::mpl::false_) const
        {
                capture_.toggle_promotion();     // no longer a pawn
                capture_.toggle_king_targets();  // can now capture kings
                auto const found_next = KingJumps(capture_).template promote_en_passant<Direction>(jumper);
                capture_.toggle_king_targets();  // can no longer capture kings
                capture_.toggle_promotion();     // now a pawn again
                return found_next;
        }

        template<typename Direction>
        bool find_next_impl(BitIndex jumper) const
        {
                return (
                        turn<Direction>(jumper) |
                        scan<Direction>(jumper)
                );
        }

        template<typename Direction>
        bool turn(BitIndex jumper) const
        {
                // tag dispatching on man turn directions
                return turn_dispatch<Direction>(jumper, typename Rules::pawn_turn_directions());
        }

        // overload for turns in all the 6 non-parallel diagonal and orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::all) const
        {
                return (
                        turn_dispatch<Direction>(jumper, rules::directions::diag()) |
                        turn_dispatch<Direction>(jumper, rules::directions::orth())
                );
        }

        // overload for turns in the 2 sideways directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::diag) const
        {
                return (
                        scan< typename mpl::rotate< Direction, angle::R090 >::type >(jumper) |
                        scan< typename mpl::rotate< Direction, angle::L090 >::type >(jumper)
                );
        }

        // overload for turns in the 1 mirrored forward direction
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::up) const
        {
                return scan< typename mpl::mirror< Direction, typename Compass::up >::type >(jumper);
        }

        // overload for turns in the 1 mirrored backward direction
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::down) const
        {
                return scan< typename mpl::mirror< Direction, typename Compass::down >::type >(jumper);
        }

        // overload for turns in the remaining 4 diagonal or orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::orth) const
        {
                return (
                        scan< typename mpl::rotate< Direction, angle::R045 >::type >(jumper) |
                        scan< typename mpl::rotate< Direction, angle::L045 >::type >(jumper) |
                        scan< typename mpl::rotate< Direction, angle::R135 >::type >(jumper) |
                        scan< typename mpl::rotate< Direction, angle::L135 >::type >(jumper)
                );
        }

        template<typename Direction>
        bool scan(BitIndex jumper) const
        {
                Advance<Board, Direction>()(jumper);
                return jump<Direction>(jumper);
        }

        template<typename Direction>
        bool jump(BitIndex jumper) const
        {
                if (!bit::is_element(jumper, capture_.template targets_with_pawn<Direction>()))
                        return false;

                capture_.make(jumper);
                precedence<Direction>(jumper); // recursively find more jumps
                capture_.undo(jumper);
                return true;
        }

        void add_pawn_jump(BitIndex dest_sq) const
        {
                // tag dispatching on ambiguity of pawn jumps
                add_pawn_jump_dispatch(dest_sq, typename Rules::is_unambiguous_pawn_jump());
        }

        // overload for pawn jumps that are always unambiguous
        void add_pawn_jump_dispatch(BitIndex dest_sq, boost::mpl::true_) const
        {
                capture_.template add_pawn_jump<Color, capture::with::pawn>(dest_sq);
        }

        // overload for pawn jumps that are potentially ambiguous
        void add_pawn_jump_dispatch(BitIndex dest_sq, boost::mpl::false_) const
        {
                auto const ambiguous = rules::is_check_jump_uniqueness<Rules>::value && capture_.is_ambiguous();
                capture_.template add_pawn_jump<Color, capture::with::pawn>(dest_sq);
                if (ambiguous)
                        capture_.remove_non_unique_back();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
