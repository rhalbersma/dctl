#pragma once
#include <cassert>                                      // assert
#include <iterator>
#include <type_traits>                                  // false_type, true_type
#include <dctl/successor/copy/impl/primary_fwd.hpp>     // copy (primary template)
#include <dctl/successor/propagate/jumps.hpp>           // Propagate
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/pieces/king.hpp>                         // king

#include <dctl/angle.hpp>                               // _deg, rotate
#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/board/iterator.hpp>                      // Increment, Next, Prev
#include <dctl/rules/traits.hpp>                        // traits
#include <dctl/utility/algorithm.hpp>
#include <dctl/ray/iterator.hpp>
#include <dctl/ray/transform.hpp>

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
                capture_{c},
                moves_{m}
        {}

        // function call operators

        template<class Set>
        void operator()(Set const& active_kings) const
        {
                // tag dispatching on relative king jump precedence
                select_dispatch(active_kings, rules::precedence::is_relative_king<Rules>{});
        }

        template<class Iterator>
        bool promote_en_passant(Iterator jumper) const
        {
                return find_next(jumper);
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
                for (auto from_sq : active_kings)
                        find(from_sq);
        }

        void find(int from_sq) const
        {
                capture_.launch(from_sq);
                branch(from_sq);
                capture_.finish(from_sq);
        }

        void branch(int from_sq) const
        {
                // tag dispatching on king jump directions
                branch_dispatch(from_sq, rules::directions::king_jump<Rules>{});
        }

        // overload for kings that jump in the 8 diagonal and orthogonal directions
        void branch_dispatch(int from_sq, rules::directions::all) const
        {
                branch_dispatch(from_sq, rules::directions::diag());
                branch_dispatch(from_sq, rules::directions::orth());
        }

        // overload for kings that jump in the 4 diagonal directions
        void branch_dispatch(int from_sq, rules::directions::diag) const
        {
                find_first(along_ray< Compass::left_up    >(from_sq));
                find_first(along_ray< Compass::right_up   >(from_sq));
                find_first(along_ray< Compass::left_down  >(from_sq));
                find_first(along_ray< Compass::right_down >(from_sq));
        }

        // overload for kings that jump in the 4 orthogonal directions
        void branch_dispatch(int from_sq, rules::directions::orth) const
        {
                find_first(along_ray< Compass::left  >(from_sq));
                find_first(along_ray< Compass::right >(from_sq));
                find_first(along_ray< Compass::up    >(from_sq));
                find_first(along_ray< Compass::down  >(from_sq));
        }

        template<class Board, int Direction>
        void find_first(ray::Iterator<Board, Direction> jumper) const
        {
                slide(jumper, capture_.template path<Direction>());
                if (capture_.template targets_with_king<Direction>(*jumper)) {
                        capture_.make(*jumper);
                        precedence(jumper);     // recursively find more jumps
                        capture_.undo(*jumper);
                }
        }

        template<class Iterator>
        void precedence(Iterator jumper) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch(jumper, rules::is_precedence<Rules>{});
        }

        // overload for no majority precedence
        template<class Iterator>
        void precedence_dispatch(Iterator jumper, std::false_type) const
        {
                ++jumper;
                if (!find_next(jumper))
                        add_king_jump(jumper);
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
                        add_king_jump(jumper);
                }
        }

        template<class Iterator>
        bool find_next(Iterator jumper) const
        {
                // tag dispatching on king jump direction reversal
                return find_next_dispatch(jumper, rules::directions::is_reversal<Rules>{});
        }

        // overload for kings that cannot reverse their capture direction
        template<class Iterator>
        bool find_next_dispatch(Iterator jumper, std::false_type) const
        {
                return land(jumper);
        }

        // overload for kings that can reverse their capture direction
        template<class Iterator>
        bool find_next_dispatch(Iterator jumper, std::true_type) const
        {
                return land(jumper) | reverse(jumper);
        }

        template<class Iterator>
        bool reverse(Iterator jumper) const
        {
                return scan(ray::rotate<180_deg>(jumper));
        }

        template<class Iterator>
        bool land(Iterator jumper) const
        {
                // tag dispatching on king jump landing range after intermediate captures
                return land_dispatch(jumper, rules::range::land<Rules>{});
        }

        // overload for kings that land immediately if the intermediate capture is a king, and slide through otherwise
        template<class Iterator>
        void land_dispatch(Iterator jumper, rules::range::distance_1K) const
        {
                if (capture_.is_king(*std::prev(jumper)))
                        land_dispatch(jumper, rules::range::distance_1());
                else
                        land_dispatch(jumper, rules::range::distance_N());
        }

        // overload for kings that can only land on the immediately adjacent square
        template<class Iterator>
        bool land_dispatch(Iterator jumper, rules::range::distance_1) const
        {
                return turn(jumper) | scan(jumper);
        }

        // overload for kings that can land on any square along the current direction
        template<class Iterator>
        bool land_dispatch(Iterator jumper, rules::range::distance_N) const
        {
                // NOTE: capture_.template path<Direction>() would be an ERROR here
                // because we need all landing squares rather than the directional launching squares subset
                assert(capture_.path(*jumper));
                auto found_next = false;
                do {
                        found_next |= turn(jumper);
                        ++jumper;
                } while (capture_.path(*jumper));
                return found_next |= jump(jumper);
        }

        template<class Iterator>
        bool turn(Iterator jumper) const
        {
                // tag dispatching on king turn directions
                return turn_dispatch(jumper, rules::directions::king_turn<Rules>{});
        }

        // overload for kings that turn in all the 6 non-parallel diagonal and orthogonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, rules::directions::all) const
        {
                return (
                        turn_dispatch(jumper, rules::directions::diag()) |
                        turn_dispatch(jumper, rules::directions::orth())
                );
        }

        // overload for kings that turn in the 2 sideways directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, rules::directions::diag) const
        {
                return (
                        scan(ray::rotate<-90_deg>(jumper)) |
                        scan(ray::rotate<+90_deg>(jumper))
                );
        }

        // overload for kings that turn in the remaining 4 diagonal or orthogonal directions
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

        template<class Board, int Direction>
        bool scan(ray::Iterator<Board, Direction> jumper) const
        {
                slide(jumper, capture_.template path<Direction>());
                return jump(jumper);
        }

        template<class Iterator, class Set>
        void slide(Iterator& jumper, Set const& path) const
        {
                // tag dispatching on king range
                slide_dispatch(jumper, path, rules::range::scan<Rules>{});
        }

        // overload for short ranged kings
        template<class Iterator, class Set>
        void slide_dispatch(Iterator& jumper, Set const& /* path */, rules::range::distance_1) const
        {
                ++jumper;
        }

        // overload for long ranged kings
        template<class Iterator, class Set>
        void slide_dispatch(Iterator& jumper, Set const& path, rules::range::distance_N) const
        {
                do ++jumper; while (path.test(*jumper));
        }

        template<class Board, int Direction>
        bool jump(ray::Iterator<Board, Direction> jumper) const
        {
                if (!capture_.template targets_with_king<Direction>(*jumper))
                        return false;

                capture_.make(*jumper);
                precedence(jumper);     // recursively find more jumps
                capture_.undo(*jumper);
                return true;
        }

        template<class Iterator>
        void add_king_jump(Iterator dest_sq) const
        {
                auto const check_duplicate = rules::is_remove_duplicates<Rules>::value && capture_.is_potential_duplicate(moves_);

                // tag dispatching on king halt after final capture
                add_king_jump_dispatch(dest_sq, check_duplicate, rules::range::halt<Rules>{});
        }

        // overload for kings that halt immediately if the final capture is a king, and slide through otherwise
        template<class Iterator>
        void add_king_jump_dispatch(Iterator dest_sq, bool check_duplicate, rules::range::distance_1K) const
        {
                if (capture_.is_king(*std::prev(dest_sq)))
                        add_king_jump_dispatch(dest_sq, check_duplicate, rules::range::distance_1());
                else
                        add_king_jump_dispatch(dest_sq, check_duplicate, rules::range::distance_N());
        }

        // overload for kings that halt immediately after the final capture
        template<class Iterator>
        void add_king_jump_dispatch(Iterator dest_sq, bool check_duplicate, rules::range::distance_1) const
        {
                add_king_jump(dest_sq, check_duplicate);
        }

        // overload for kings that slide through after the final capture
        template<class Iterator>
        void add_king_jump_dispatch(Iterator dest_sq, bool check_duplicate, rules::range::distance_N) const
        {
                // NOTE: capture_.template path<Direction>() would be an ERROR here
                // because we need all halting squares rather than the directional launching squares subset
                assert(capture_.path(*dest_sq));
                do {
                        add_king_jump(dest_sq, check_duplicate);
                        ++dest_sq;
                } while (capture_.path(*dest_sq));
        }

        template<class Iterator>
        void add_king_jump(Iterator dest_sq, bool check_duplicate) const
        {
                // tag dispatching on promotion condition
                add_king_jump_dispatch(dest_sq, rules::phase::promotion<Rules>{});
                if (check_duplicate && util::is_duplicate_back(moves_))
                        moves_.pop_back();
        }

        // overload for pawns that promote apres-fini
        template<class Iterator>
        void add_king_jump_dispatch(Iterator dest_sq, rules::phase::apres_fini) const
        {
                capture_.template add_king_jump<Color>(*dest_sq, moves_);
        }

        // overload for pawns that promote en-passant
        template<class Iterator>
        void add_king_jump_dispatch(Iterator dest_sq, rules::phase::en_passant) const
        {
                if (!capture_.is_promotion())
                        capture_.template add_king_jump<Color>(*dest_sq, moves_);
                else
                        capture_.template add_pawn_jump<Color, with::king>(*dest_sq, moves_);
        }

        template<int Direction>
        static ray::Iterator<Board, Direction> along_ray(int sq)
        {
                return ray::make_iterator<Board, Direction>(sq);
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
