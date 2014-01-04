#pragma once
#include <cassert>                                      // assert
#include <iterator>
#include <type_traits>                                  // false_type, true_type
#include <dctl/successor/generate/impl/primary_fwd.hpp> // generate (primary template)
#include <dctl/successor/generate/impl/king_jumps.hpp>  // promote_en_passant
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/pieces/pawn.hpp>                         // pawn
#include <dctl/pieces/king.hpp>


#include <dctl/angle.hpp>                               // _deg, rotate, mirror
#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/position/promotion.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/algorithm.hpp>
#include <dctl/ray/iterator.hpp>
#include <dctl/ray/transform.hpp>
#include <dctl/wave/iterator.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for pawn jumps generation
template<bool Color, class Position, class Sequence>
struct generate<Color, pieces::pawn, select::jumps, Position, Sequence>
{
public:
        // enforce reference semantics
        generate(generate const&) = delete;
        generate& operator=(generate const&) = delete;

private:
        using KingJumps = generate<Color, pieces::king, select::jumps, Position, Sequence>;
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Set = typename Board::set_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::jumps, Position>;

        // representation

        State& capture_;
        Sequence& moves_;

public:
        // structors

        explicit generate(State& c, Sequence& m)
        :
                capture_{c},
                moves_{m}
        {}

        // function call operators

        void operator()(Set const& active_pawns) const
        {
                // tag dispatching on whether pawns can capture kings
                if (active_pawns.empty())
                        return;

                select_dispatch(active_pawns, rules::can_jump<Rules, pieces::pawn, pieces::king>());
        }

private:
        // overload for pawns that can capture kings
        void select_dispatch(Set const& active_pawns, std::true_type) const
        {
                branch(active_pawns);
        }

        // overload for pawns that cannot capture kings
        void select_dispatch(Set const& active_pawns, std::false_type) const
        {
                capture_.toggle_king_targets();
                branch(active_pawns);
                capture_.toggle_king_targets();
        }

        void branch(Set const& active_pawns) const
        {
                // tag dispatching on pawn jump directions
                branch_dispatch(active_pawns, rules::directions::pawn_jump<Rules>{});
        }

        // overload for pawns that jump in the 8 diagonal and orthogonal directions
        void branch_dispatch(Set const& active_pawns, rules::directions::all) const
        {
                branch_dispatch(active_pawns, rules::directions::diag());
                branch_dispatch(active_pawns, rules::directions::orth());
        }

        // overload for pawns that jump in the 4 diagonal directions
        void branch_dispatch(Set const& active_pawns, rules::directions::diag) const
        {
                branch_dispatch(active_pawns, rules::directions::up  ());
                branch_dispatch(active_pawns, rules::directions::down());
        }

        // overload for pawns that jump in the 2 forward diagonal directions
        void branch_dispatch(Set const& active_pawns, rules::directions::up) const
        {
                serialize< Compass::left_up  >(active_pawns);
                serialize< Compass::right_up >(active_pawns);
        }

        // overload for pawns that jump in the 2 backward diagonal directions
        void branch_dispatch(Set const& active_pawns, rules::directions::down) const
        {
                serialize< Compass::left_down  >(active_pawns);
                serialize< Compass::right_down >(active_pawns);
        }

        // overload for pawns that jump in the 4 orthogonal directions
        void branch_dispatch(Set const& active_pawns, rules::directions::orth) const
        {
                serialize< Compass::left  >(active_pawns);
                serialize< Compass::right >(active_pawns);
                serialize< Compass::up    >(active_pawns);
                serialize< Compass::down  >(active_pawns);
        }

        template<int Direction>
        void serialize(Set const& active_pawns) const
        {
                auto const jumpers = active_pawns & *std::prev(along_wave<Direction>(capture_.template targets_with_pawn<Direction>()));
                for (auto const& from_sq : jumpers)
                        find(along_ray<Direction>(from_sq));
        }

        template<class Iterator>
        void find(Iterator jumper) const
        {
                capture_.launch(*jumper);
                find_first(std::next(jumper));
                capture_.finish(*jumper);
        }

        template<class Iterator>
        void find_first(Iterator jumper) const
        {
                capture_.make(*jumper);
                precedence(jumper);     // recursively find more jumps
                capture_.undo(*jumper);
        }

        template<class Iterator>
        void precedence(Iterator jumper) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch(jumper, typename rules::is_precedence<Rules>{});
        }

        // overload for no majority precedence
        template<class Iterator>
        void precedence_dispatch(Iterator jumper, std::false_type) const
        {
                ++jumper;
                if (!find_next(jumper))
                        add_pawn_jump(jumper);
        }

        // overload for majority precedence
        template<class Iterator>
        void precedence_dispatch(Iterator jumper, std::true_type) const
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

        template<class Iterator>
        bool find_next(Iterator jumper) const
        {
                // tag dispatching on promotion condition
                return find_next_dispatch(jumper, rules::phase::promotion<Rules>{});
        }

        // overload for pawns that promote apres-fini
        template<class Iterator>
        bool find_next_dispatch(Iterator jumper, rules::phase::apres_fini) const
        {
                return find_next_impl(jumper);
        }

        // overload for pawns that promote en-passant
        template<class Iterator>
        bool find_next_dispatch(Iterator jumper, rules::phase::en_passant) const
        {
                return is_promotion(*jumper) ?
                        promote_en_passant(jumper) :
                        find_next_impl(jumper)
                ;
        }

        template<class Iterator>
        bool find_next_impl(Iterator jumper) const
        {
                return turn(jumper) | scan(jumper);
        }

        template<class Iterator>
        bool promote_en_passant(Iterator jumper) const
        {
                // tag dispatching on whether pawns can capture kings
                return promote_en_passant_dispatch(jumper, rules::can_jump<Rules, pieces::pawn, pieces::king>());
        }

        // overload for pawns that can capture kings
        template<class Iterator>
        bool promote_en_passant_dispatch(Iterator jumper, std::true_type) const
        {
                capture_.toggle_promotion();
                auto const found_next = KingJumps{capture_, moves_}.promote_en_passant(jumper);
                capture_.toggle_promotion();
                return found_next;
        }

        // overload for pawns that cannot capture kings
        template<class Iterator>
        bool promote_en_passant_dispatch(Iterator jumper, std::false_type) const
        {
                capture_.toggle_promotion();    // no longer a pawn
                capture_.set_king_targets();    // can now capture kings
                auto const found_next = KingJumps{capture_, moves_}.promote_en_passant(jumper);
                capture_.clear_king_targets();  // can no longer capture kings
                capture_.toggle_promotion();    // now a pawn again
                return found_next;
        }

        template<class Iterator>
        bool turn(Iterator jumper) const
        {
                // tag dispatching on pawn turn directions
                return turn_dispatch(jumper, rules::directions::pawn_turn<Rules>{});
        }

        // overload for pawns that turn in all the 6 non-parallel diagonal and orthogonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, rules::directions::all) const
        {
                return (
                        turn_dispatch(jumper, rules::directions::diag()) |
                        turn_dispatch(jumper, rules::directions::orth())
                );
        }

        // overload for pawns that turn in the 2 sideways directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, rules::directions::diag) const
        {
                return (
                        scan(ray::rotate<-90_deg>(jumper)) |
                        scan(ray::rotate<+90_deg>(jumper))
                );
        }

        // overload for pawns that turn in the 1 mirrored forward direction
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, rules::directions::up) const
        {
                return scan(ray::mirror<Compass::up>(jumper));
        }

        // overload for pawns that turn in the 1 mirrored backward direction
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, rules::directions::down) const
        {
                return scan(ray::mirror<Compass::down>(jumper));
        }

        // overload for pawns that turn in the remaining 4 diagonal or orthogonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, rules::directions::orth) const
        {
                return (
                        scan(ray::rotate< -45_deg>(jumper)) |
                        scan(ray::rotate< +45_deg>(jumper)) |
                        scan(ray::rotate<-135_deg>(jumper)) |
                        scan(ray::rotate<+135_deg>(jumper))
                );
        }

        template<class Iterator>
        bool scan(Iterator jumper) const
        {
                ++jumper;
                return jump(jumper);
        }

        template<int Direction>
        bool jump(ray::Iterator<Board, Direction> jumper) const
        {
                if (!capture_.template targets_with_pawn<Direction>(*jumper))
                        return false;

                capture_.make(*jumper);
                precedence(jumper);  // recursively find more jumps
                capture_.undo(*jumper);
                return true;
        }

        template<class Iterator>
        void add_pawn_jump(Iterator dest) const
        {
                // tag dispatching on ambiguity of pawn jumps
                add_pawn_jump_dispatch(dest, rules::is_unambiguous_pawn_jump<Rules>{});
        }

        // overload for pawn jumps that are always unambiguous
        template<class Iterator>
        void add_pawn_jump_dispatch(Iterator dest, std::true_type) const
        {
                capture_.template add_pawn_jump<Color, with::pawn>(*dest, moves_);
        }

        // overload for pawn jumps that are potentially ambiguous
        template<class Iterator>
        void add_pawn_jump_dispatch(Iterator dest, std::false_type) const
        {
                auto const check_duplicate = rules::is_remove_duplicates<Rules>::value && capture_.is_potential_duplicate(moves_);
                capture_.template add_pawn_jump<Color, with::pawn>(*dest, moves_);
                if (check_duplicate && util::is_duplicate_back(moves_))
                        moves_.pop_back();
        }

        template<int Direction>
        static wave::Iterator<Board, Direction> along_wave(Set const& s)
        {
                return wave::make_iterator<Board, Direction>(s);
        }

        template<int Direction>
        static ray::Iterator<Board, Direction> along_ray(int sq)
        {
                return ray::make_iterator<Board, Direction>(sq);
        }

        static bool is_promotion(int sq)
        {
                return dctl::is_promotion<Color, Board>(sq);
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
