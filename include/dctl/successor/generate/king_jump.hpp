#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/tracker.hpp>            // Propagate (jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/pieces/king.hpp>                         // king

#include <dctl/angle.hpp>                               // _deg, rotate, inverse
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/ray.hpp>                                 // make_iterator, rotate, mirror
#include <dctl/rule_traits.hpp>
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t
#include <dctl/utility/algorithm.hpp>                   // is_duplicate_back
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // false_type, true_type

namespace dctl {
namespace successor {

// partial specialization for king jumps generation
template<bool Color, class Position, class Sequence>
class Generate<Color, pieces::king, select::jump, Position, Sequence>
{
public:
        // enforce reference semantics
        Generate(Generate const&) = delete;
        Generate& operator=(Generate const&) = delete;

private:
        using Rules = rules_type_t<Position>;
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;
        using Move = value_type_t<Sequence>;
        using State = Propagate<Color, Position>;

        static constexpr auto orientation = orientation_v<Board, Color>;

        template<class Iterator>
        static constexpr auto direction_v = rotate(ray::direction_v<Iterator>, inverse(orientation));

        // representation

        State& tracker_;
        Sequence& moves_;

public:
        // constructors

        Generate(State& c, Sequence& m)
        :
                tracker_{c},
                moves_{m}
        {}

        // function call operators

        auto operator()(Set const& active_kings) const
        {
                tracker_.toggle_with_king();
                serialize(active_kings);
                tracker_.toggle_with_king();
        }

        template<class Iterator>
        bool promote_en_passant(Iterator jumper) const
        {
                // tag dispatching on whether pawns can capture kings
                return can_jump_dispatch(jumper, is_pawn_jump_king_t<Rules>{});
        }

private:
        void serialize(Set const& active_kings) const
        {
                for (auto&& from_sq : active_kings) {
                        tracker_.launch(from_sq);
                        branch(from_sq);
                        tracker_.finish();
                }
        }

        void branch(int from_sq) const
        {
                // tag dispatching on king jump directions
                branch_dispatch(from_sq, is_orthogonal_jump_t<Rules>{});
        }

        // kings that jump in the 4 diagonal directions
        void branch_dispatch(int from_sq, std::false_type) const
        {
                find_first(along_ray<left_up   (orientation)>(from_sq));
                find_first(along_ray<right_up  (orientation)>(from_sq));
                find_first(along_ray<left_down (orientation)>(from_sq));
                find_first(along_ray<right_down(orientation)>(from_sq));
        }

        // kings that jump in the 8 diagonal and orthogonal directions
        void branch_dispatch(int from_sq, std::true_type) const
        {
                find_first(along_ray<up        (orientation)>(from_sq));
                find_first(along_ray<left_up   (orientation)>(from_sq));
                find_first(along_ray<right_up  (orientation)>(from_sq));
                find_first(along_ray<left      (orientation)>(from_sq));
                find_first(along_ray<right     (orientation)>(from_sq));
                find_first(along_ray<left_down (orientation)>(from_sq));
                find_first(along_ray<right_down(orientation)>(from_sq));
                find_first(along_ray<down      (orientation)>(from_sq));
        }

        template<class Iterator>
        void find_first(Iterator jumper) const
        {
                slide(jumper, tracker_.template path<ray::direction_v<Iterator>>());
                if (is_onboard(std::next(jumper)) && tracker_.targets_with_king(jumper))
                        capture(jumper);
        }

        template<class Iterator>
        void capture(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                tracker_.capture(*jumper);
                land(std::next(jumper));
                tracker_.release();
        }

        template<class Iterator>
        void land(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                tracker_.visit(*jumper);
                if (!find_next(jumper))
                        add(jumper);
                tracker_.leave();
        }

        // pawns that can capture kings
        template<class Iterator>
        bool can_jump_dispatch(Iterator jumper, std::true_type) const
        {
                tracker_.toggle_promotion();
                auto const found_next = find_next(jumper);
                tracker_.toggle_promotion();
                return found_next;
        }

        // pawns that cannot capture kings
        template<class Iterator>
        bool can_jump_dispatch(Iterator jumper, std::false_type) const
        {
                tracker_.toggle_promotion();    // no longer a pawn
                tracker_.set_king_targets();    // can now capture kings
                auto const found_next = find_next(jumper);
                tracker_.clear_king_targets();  // can no longer capture kings
                tracker_.toggle_promotion();    // now a pawn again
                return found_next;
        }

        template<class Iterator>
        bool find_next(Iterator jumper) const
        {
                // tag dispatching on king jump direction reversal
                return find_next_dispatch(jumper, is_reversible_king_jump_direction_t<Rules>{});
        }

        // kings that cannot reverse their capture direction
        template<class Iterator>
        bool find_next_dispatch(Iterator jumper, std::false_type) const
        {
                return explore(jumper);
        }

        // kings that can reverse their capture direction
        template<class Iterator>
        bool find_next_dispatch(Iterator jumper, std::true_type) const
        {
                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
                return explore(jumper) | reverse(jumper);
        }

        template<class Iterator>
        bool reverse(Iterator jumper) const
        {
                return scan(ray::rotate<180_deg>(jumper));
        }

        template<class Iterator>
        bool explore(Iterator jumper) const
        {
                // tag dispatching on king jump landing range after intermediate captures
                return explore_dispatch(jumper, is_long_ranged_land_after_piece_t<Rules>{});
        }

        // kings that can land on any square along the current direction
        template<class Iterator>
        bool explore_dispatch(Iterator jumper, std::true_type) const
        {
                // CORRECTNESS: tracker_.template path<Direction>() would be an ERROR here
                // because we need all landing squares rather than the directional launching squares subset
                assert(is_onboard(jumper) && tracker_.path(*jumper));
                auto found_next = false;
                do {
                        found_next |= turn(jumper);
                        ++jumper;
                } while (is_onboard(jumper) && tracker_.path(*jumper));
                return found_next |= is_en_prise(jumper);
        }

        // kings that can only land on the immediately adjacent square
        template<class Iterator>
        bool explore_dispatch(Iterator jumper, std::false_type) const
        {
                return scan(jumper) | turn(jumper);
        }

        template<class Iterator>
        bool turn(Iterator jumper) const
        {
                // tag dispatching on king turn directions
                return turn_dispatch(jumper, is_orthogonal_jump_t<Rules>{});
        }

        // kings that jump in the 4 diagonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, std::false_type) const
        {
                static_assert(is_diagonal(direction_v<Iterator>), "");

                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
                return
                        scan(ray::rotate<+90_deg>(jumper)) |
                        scan(ray::rotate<-90_deg>(jumper))
                ;
        }

        // kings that jump in the 8 diagonal and orthogonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, std::true_type) const
        {
                static_assert(is_diagonal(direction_v<Iterator>) || is_orthogonal(direction_v<Iterator>), "");

                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
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
                slide(jumper, tracker_.template path<ray::direction_v<Iterator>>());
                return is_en_prise(jumper);
        }

        template<class Iterator>
        void slide(Iterator& jumper, Set const& path) const
        {
                assert(is_onboard(jumper));

                // tag dispatching on king range
                slide_dispatch(jumper, path, is_long_ranged_king_t<Rules>{});
        }

        // short ranged kings
        template<class Iterator>
        void slide_dispatch(Iterator& jumper, Set const& /* path */, std::false_type) const
        {
                ++jumper;
        }

        // long ranged kings
        template<class Iterator>
        void slide_dispatch(Iterator& jumper, Set const& path, std::true_type) const
        {
                do ++jumper; while (is_onboard(jumper) && path.test(*jumper));
        }

        template<class Iterator>
        bool is_en_prise(Iterator jumper) const
        {
                if (!(is_onboard(jumper) && tracker_.targets_with_king(jumper)))
                        return false;

                capture(jumper);
                return true;
        }

        template<class Iterator>
        void add(Iterator dest_sq) const
        {
                // tag dispatching on king halt after final capture
                halt_dispatch(dest_sq, std::pair<is_long_ranged_land_after_piece_t<Rules>, is_directly_halt_after_final_king_t<Rules>>{});
        }

        // kings that halt immediately if the final capture is a king, and slide through otherwise
        template<class Iterator>
        void halt_dispatch(Iterator dest_sq, std::pair<std::true_type, std::true_type>) const
        {
                assert(is_onboard(std::prev(dest_sq)));
                if (tracker_.is_king(*std::prev(dest_sq)))
                        halt_dispatch(dest_sq, std::pair<std::false_type, std::true_type>{});
                else
                        halt_dispatch(dest_sq, std::pair<std::true_type, std::false_type>{});
        }

        // kings that halt immediately after the final capture
        template<class Iterator, class B>
        void halt_dispatch(Iterator dest_sq, std::pair<std::false_type, B>) const
        {
                add_jump(dest_sq);
        }

        // kings that slide through after the final capture
        template<class Iterator>
        void halt_dispatch(Iterator dest_sq, std::pair<std::true_type, std::false_type>) const
        {
                // NOTE: tracker_.template path<Direction>() would be an ERROR here
                // because we need all halting squares rather than the directional launching squares subset
                assert(is_onboard(dest_sq) && tracker_.path(*dest_sq));
                do add_jump(dest_sq++); while (is_onboard(dest_sq) && tracker_.path(*dest_sq));
        }

        template<class Iterator>
        void add_jump(Iterator dest_sq) const
        {
                assert(is_onboard(dest_sq));

                // tag dispatching on promotion condition
                promotion_dispatch(dest_sq, is_en_passant_promotion_t<Rules>{});
        }

        // pawns that promote apres-fini
        template<class Iterator>
        void promotion_dispatch(Iterator dest_sq, std::false_type) const
        {
                tracker_.add_king_jump(*dest_sq, moves_);
        }

        // pawns that promote en-passant
        template<class Iterator>
        void promotion_dispatch(Iterator dest_sq, std::true_type) const
        {
                if (!tracker_.is_promotion())
                        tracker_.add_king_jump(*dest_sq, moves_);
                else
                        tracker_.template add_pawn_jump<with::king>(*dest_sq, moves_);
        }

        template<int Direction>
        static ray::Iterator<Board, Direction> along_ray(int sq)
        {
                return ray::make_iterator<Board, Direction>(sq);
        }
};

}       // namespace successor
}       // namespace dctl
