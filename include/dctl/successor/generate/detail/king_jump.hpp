#pragma once
#include <dctl/successor/generate/detail/primary_fwd.hpp>       // Generate (primary template)
#include <dctl/successor/propagate/jump.hpp>                    // Propagate (jump specialization)
#include <dctl/successor/select/jump.hpp>                       // jump
#include <dctl/pieces/king.hpp>                                 // king

#include <dctl/angle.hpp>                                       // _deg, rotate
#include <dctl/board/compass.hpp>                               // Compass
#include <dctl/ray.hpp>                                         // make_iterator, rotate
#include <dctl/rules/traits.hpp>                                // traits
#include <dctl/utility/algorithm.hpp>
#include <cassert>                                              // assert
#include <iterator>                                             // prev
#include <type_traits>                                          // false_type, true_type

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps generation
template<bool Color, class Position, class Sequence>
struct Generate<Color, pieces::king, select::jump, Position, Sequence>
{
public:
        // enforce reference semantics
        Generate(Generate const&) = delete;
        Generate& operator=(Generate const&) = delete;

private:
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Set = typename Board::set_type;
        using Move = typename Sequence::value_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::jump, Position>;

        // representation

        State& capture_;
        Sequence& moves_;

public:
        // structors

        Generate(State& c, Sequence& m)
        :
                capture_{c},
                moves_{m}
        {}

        // function call operators

        void operator()(Set const& active_kings) const
        {
                // tag dispatching on relative king jump precedence
                select_dispatch(active_kings, rules::is_relative_king_jump_precedence_t<Rules>{});
        }

        template<class Iterator>
        bool promote_en_passant(Iterator jumper) const
        {
                return !is_finished(jumper);
        }

private:
        // overload for no relative king jump precedence
        void select_dispatch(Set const& active_kings, std::false_type) const
        {
                serialize(active_kings);
        }

        // overload for relative king jump precedence
        void select_dispatch(Set const& active_kings, std::true_type) const
        {
                capture_.toggle_with_king();
                serialize(active_kings);
                capture_.toggle_with_king();
        }

        void serialize(Set const& active_kings) const
        {
                for (auto&& from_sq : active_kings) {
                        capture_.launch(from_sq);
                        branch(from_sq);
                        capture_.finish(from_sq);
                }
        }

        void branch(int from_sq) const
        {
                // tag dispatching on king jump directions
                branch_dispatch(from_sq, is_orthogonal_jump_t<Rules>{});
        }

        // overload for kings that jump in the 4 diagonal directions
        void branch_dispatch(int from_sq, std::false_type) const
        {
                find(along_ray<Compass::left_up   >(from_sq));
                find(along_ray<Compass::right_up  >(from_sq));
                find(along_ray<Compass::left_down >(from_sq));
                find(along_ray<Compass::right_down>(from_sq));
        }

        // overload for kings that jump in the 8 diagonal and orthogonal directions
        void branch_dispatch(int from_sq, std::true_type) const
        {
                find(along_ray<Compass::up        >(from_sq));
                find(along_ray<Compass::left_up   >(from_sq));
                find(along_ray<Compass::right_up  >(from_sq));
                find(along_ray<Compass::left      >(from_sq));
                find(along_ray<Compass::right     >(from_sq));
                find(along_ray<Compass::left_down >(from_sq));
                find(along_ray<Compass::right_down>(from_sq));
                find(along_ray<Compass::down      >(from_sq));
        }

        template<class Iterator>
        void find(Iterator jumper) const
        {
                slide(jumper, capture_.template path<ray::direction<Iterator>::value>());
                if (capture_.targets_with_king(jumper))
                        explore(jumper);        // recursively find more jumps
        }

        template<class Iterator>
        void explore(Iterator jumper) const
        {
                capture_.make(*jumper);
                add_and_continue(std::next(jumper));
                capture_.undo(*jumper);
        }

        template<class Iterator>
        void add_and_continue(Iterator jumper) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch(jumper, rules::is_precedence<Rules>{});
        }

        // overload for no majority precedence
        template<class Iterator>
        void precedence_dispatch(Iterator jumper, std::false_type) const
        {
                if (is_finished(jumper))
                        add(jumper);
        }

        // overload for majority precedence
        template<class Iterator>
        void precedence_dispatch(Iterator jumper, std::true_type) const
        {
                if (is_finished(jumper) && capture_.greater_equal()) {
                        if (capture_.not_equal_to()) {
                                capture_.improve();
                                moves_.clear();
                        }
                        add(jumper);
                }
        }

        template<class Iterator>
        bool is_finished(Iterator jumper) const
        {
                // tag dispatching on king jump direction reversal
                return !find_next_dispatch(jumper, is_reversible_king_jump_direction_t<Rules>{});
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
        bool land_dispatch(Iterator jumper, rules::range::distance_1K) const
        {
                return
                        capture_.is_king(*std::prev(jumper)) ?
                        land_dispatch(jumper, rules::range::distance_1{}) :
                        land_dispatch(jumper, rules::range::distance_N{})
                ;
        }

        // overload for kings that can only land on the immediately adjacent square
        template<class Iterator>
        bool land_dispatch(Iterator jumper, rules::range::distance_1) const
        {
                return scan(jumper) | turn(jumper);
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
                return found_next |= is_en_prise(jumper);
        }

        template<class Iterator>
        bool turn(Iterator jumper) const
        {
                // tag dispatching on king turn directions
                return turn_dispatch(jumper, is_orthogonal_jump_t<Rules>{});
        }

        // overload for kings that jump in the 4 diagonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, std::false_type) const
        {
                return
                        scan(ray::rotate<+90_deg>(jumper)) |
                        scan(ray::rotate<-90_deg>(jumper))
                ;
        }

        // overload for kings that jump in the 8 diagonal and orthogonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, std::true_type) const
        {
                return
                        scan(ray::rotate< +45_deg>(jumper)) |
                        scan(ray::rotate< -45_deg>(jumper)) |
                        scan(ray::rotate< +90_deg>(jumper)) |
                        scan(ray::rotate< -90_deg>(jumper)) |
                        scan(ray::rotate<+135_deg>(jumper)) |
                        scan(ray::rotate<-135_deg>(jumper))
                ;
        }

        template<class Iterator>
        bool scan(Iterator jumper) const
        {
                slide(jumper, capture_.template path<ray::direction<Iterator>::value>());
                return is_en_prise(jumper);
        }

        template<class Iterator>
        void slide(Iterator& jumper, Set const& path) const
        {
                // tag dispatching on king range
                slide_dispatch(jumper, path, is_long_ranged_king_t<Rules>{});
        }

        // overload for short ranged kings
        template<class Iterator>
        void slide_dispatch(Iterator& jumper, Set const& /* path */, std::false_type) const
        {
                ++jumper;
        }

        // overload for long ranged kings
        template<class Iterator>
        void slide_dispatch(Iterator& jumper, Set const& path, std::true_type) const
        {
                do ++jumper; while (path.test(*jumper));
        }

        template<class Iterator>
        bool is_en_prise(Iterator jumper) const
        {
                if (!capture_.targets_with_king(jumper))
                        return false;

                explore(jumper);        // recursively find more jumps
                return true;
        }

        template<class Iterator>
        void add(Iterator dest_sq) const
        {
                auto const check_duplicate = rules::is_remove_duplicates<Rules>::value && capture_.is_potential_duplicate(moves_);

                // tag dispatching on king halt after final capture
                halt_dispatch(dest_sq, check_duplicate, rules::range::halt<Rules>{});
        }

        // overload for kings that halt immediately if the final capture is a king, and slide through otherwise
        template<class Iterator>
        void halt_dispatch(Iterator dest_sq, bool check_duplicate, rules::range::distance_1K) const
        {
                if (capture_.is_king(*std::prev(dest_sq)))
                        halt_dispatch(dest_sq, check_duplicate, rules::range::distance_1{});
                else
                        halt_dispatch(dest_sq, check_duplicate, rules::range::distance_N{});
        }

        // overload for kings that halt immediately after the final capture
        template<class Iterator>
        void halt_dispatch(Iterator dest_sq, bool check_duplicate, rules::range::distance_1) const
        {
                add_jump(dest_sq, check_duplicate);
        }

        // overload for kings that slide through after the final capture
        template<class Iterator>
        void halt_dispatch(Iterator dest_sq, bool check_duplicate, rules::range::distance_N) const
        {
                // NOTE: capture_.template path<Direction>() would be an ERROR here
                // because we need all halting squares rather than the directional launching squares subset
                assert(capture_.path(*dest_sq));
                do add_jump(dest_sq++, check_duplicate); while (capture_.path(*dest_sq));
        }

        template<class Iterator>
        void add_jump(Iterator dest_sq, bool check_duplicate) const
        {
                // tag dispatching on promotion condition
                promotion_dispatch(dest_sq, is_en_passant_promotion_t<Rules>{});
                if (check_duplicate && util::is_duplicate_back(moves_))
                        moves_.pop_back();
        }

        // overload for pawns that promote apres-fini
        template<class Iterator>
        void promotion_dispatch(Iterator dest_sq, std::false_type) const
        {
                capture_.template add_king_jump<Color>(*dest_sq, moves_);
        }

        // overload for pawns that promote en-passant
        template<class Iterator>
        void promotion_dispatch(Iterator dest_sq, std::true_type) const
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

}       // namespace detail
}       // namespace successor
}       // namespace dctl
